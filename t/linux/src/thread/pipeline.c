/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "kmisc.h"
#include "pipeline.h"


typedef struct pipeline_stage_s pipeline_stage_t;


struct pipeline_s {
  pipeline_stage_t *head;
  pipeline_stage_t *tail;
  int               stages;
  pthread_mutex_t   mutex;
  pthread_cond_t    finished;
  int               active;
};


struct pipeline_stage_s {
  pthread_t         thread;
  int               index;
  pthread_mutex_t   mutex;
  pthread_cond_t    avail;
  pthread_cond_t    ready;
  int               data_ready;
  void             *data;
  pipeline_stage_t *next;
};


static void *_pipeline_stage_tail(void *);
static void *_pipeline_stage(void *);
static int   _pipeline_send(pipeline_stage_t *stage, void *data);


pipeline_t *
pipeline_create(int stages)
{
  pipeline_t       *pl;
  pipeline_stage_t *stage;
  int               i;
  int               n;
  int               res;

  pl = (pipeline_t *) malloc(sizeof(*pl) + stages * sizeof(pipeline_stage_t));
  if (pl == NULL)
    return NULL;

  pl->stages = 0;
  pl->active = 0;
  if (pthread_mutex_init(&pl->mutex, NULL) != 0) {
    free(pl);
    return NULL;
  }

  if (pthread_cond_init(&pl->finished, NULL) != 0) {
    pthread_mutex_destroy(&pl->mutex);
    free(pl);
    return NULL;
  }

  stage = ((pipeline_stage_t *) (pl + 1)) + (stages - 1);
  pl->head = NULL;
  pl->tail = NULL;

  for (i = stages; i > 0; i--, stage--) {
    res = pthread_mutex_init(&stage->mutex, NULL);
    if (res != 0)
      break;

    res = pthread_cond_init(&stage->avail, NULL);
    if (res != 0) {
      pthread_mutex_destroy(&stage->mutex);
      break;
    }

    res = pthread_cond_init(&stage->ready, NULL);
    if (res != 0) {
      pthread_mutex_destroy(&stage->mutex);
      pthread_cond_destroy(&stage->avail);
      break;
    }

    stage->data_ready = 0;
    stage->index = i;
    pl->head = stage;
    pl->stages++;

    if (i == stages) {
      pl->tail = stage;
      stage->next = NULL;
      res = pthread_create(&stage->thread, NULL,
                           _pipeline_stage_tail, (void *) pl);

    } else {
      stage->next = stage + 1;
      res = pthread_create(&stage->thread, NULL,
                           _pipeline_stage, (void *) stage);
    }

    if (res != 0) {
      i--;
      break;
    }
  }

  if (res != 0) {
    n = i;
    for (i = stages; i > n; i--, stage++) {
      pthread_mutex_destroy(&stage->mutex);
      pthread_cond_destroy(&stage->avail);
      pthread_cond_destroy(&stage->ready);
    }

    pthread_mutex_destroy(&pl->mutex);
    pthread_cond_destroy(&pl->finished);
    free(pl);
    return NULL;
  }

  return pl;
}


void
pipeline_destroy(pipeline_t *pl)
{
  pipeline_stage_t *stage;
  if (pl == NULL)
    return ;

  for (stage = pl->head; stage != NULL; stage = stage->next) {
    pthread_mutex_destroy(&stage->mutex);
    pthread_cond_destroy(&stage->avail);
    pthread_cond_destroy(&stage->ready);
  }

  pthread_mutex_destroy(&pl->mutex);
  pthread_cond_destroy(&pl->finished);
  free(pl);
}


kerrno_t
pipeline_start(pipeline_t *pl, void *data)
{
  if (pthread_mutex_lock(&pl->mutex) != 0)
    return KERROR;

  pl->active++;
  if (pthread_mutex_unlock(&pl->mutex) != 0)
    return KERROR;

  return _pipeline_send(pl->head, data);
}


kerrno_t
pipeline_wait(pipeline_t *pl)
{
  kerrno_t res;

  if (pthread_mutex_lock(&pl->mutex) != 0)
    return KERROR;

  while (pl->active) {
    res = pthread_cond_wait(&pl->finished, &pl->mutex);
    if (res != 0)
      break;
  }

  res += pthread_mutex_unlock(&pl->mutex);
  if (res != KSUCCESS)
    return KERROR;
  return KSUCCESS;
}


static void *
_pipeline_stage_tail(void *arg)
{
  pipeline_t       *pl = (pipeline_t *) arg;
  pipeline_stage_t *stage = pl->tail;
  int               res;
  kerrno_t          ret;

  if (pthread_mutex_lock(&stage->mutex) != 0)
    kerror("state_tail lock error");

  while (KTRUE) {
    while (!stage->data_ready) {
      if (pthread_cond_wait(&stage->avail, &stage->mutex)) {
        pthread_mutex_unlock(&stage->mutex);
        kerror("stage_tail cond wait error");
      }
    }

    res = (int) (long) stage->data;
    res += stage->index;
    printf("    result: %d\n", res);
    fflush(stdout);

    ret = pthread_mutex_lock(&pl->mutex);
    stage->data_ready = 0;
    if (!(--pl->active)) {
      ret += pthread_cond_signal(&pl->finished);
    }
    ret += pthread_mutex_unlock(&pl->mutex);
  }

  ret += pthread_mutex_unlock(&stage->mutex); // this is not nesscessary
  if (ret != 0)
    kerror("stage_tail unlock error");
}


static void *
_pipeline_stage(void *arg)
{
  pipeline_stage_t *stage = (pipeline_stage_t *) arg;

  if (pthread_mutex_lock(&stage->mutex))
    kerror("pipeline stage lock mutex error");

  while (KTRUE) {
    while (!stage->data_ready) {
      if (pthread_cond_wait(&stage->avail, &stage->mutex))
        kerror("pipeline stage wait avail cond error");
    }

    stage->data = ((int) stage->data) + stage->index;
    _pipeline_send(stage->next, stage->data);

    stage->data_ready = 0;
    if (pthread_cond_signal(&stage->ready))
      kerror("pipeline stage signal ready error");
  }

  if (pthread_mutex_unlock(&stage->mutex))
    kerror("pipeline stage unlock mutex error");
}


static kerrno_t
_pipeline_send(pipeline_stage_t *stage, void *data)
{
  if (pthread_mutex_lock(&stage->mutex) != 0)
    return KERROR;

  while (stage->data_ready) {
    if (pthread_cond_wait(&stage->ready, &stage->mutex) != 0) {
      pthread_mutex_unlock(&stage->mutex);
      return KERROR;
    }
  }

  stage->data = data;
  stage->data_ready = 1;
  if (pthread_cond_signal(&stage->avail) != 0) {
    pthread_mutex_unlock(&stage->mutex);
    return KERROR;
  }

  if (pthread_mutex_unlock(&stage->mutex) != 0)
    return KERROR;

  return KSUCCESS;
}

