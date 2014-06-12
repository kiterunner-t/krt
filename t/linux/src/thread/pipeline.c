/**
 * Copyleft (C) KRT, 2013-2014 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "kmisc.h"
#include "pipeline.h"


typedef struct pipeline_stage_s pipeline_stage_t;
typedef struct pipeline_msg_s   pipeline_msg_t;


struct pipeline_s {
  pipeline_stage_t *head;
  pipeline_stage_t *tail;
  int               stages;

  pthread_mutex_t   mutex;
  pthread_cond_t    finished;
  int               active;
};


struct pipeline_msg_s {
  int   exit;
  void *data;
};


struct pipeline_stage_s {
  pthread_t         thread;
  int               index;

  pthread_mutex_t   mutex;
  pthread_cond_t    avail;
  pthread_cond_t    ready;
  int               data_ready;
  pipeline_msg_t    msg;

  pipeline_stage_t *next;

  pipeline_stage_process_pf  proc;
};


static void *_pipeline_stage_tail(void *);
static void *_pipeline_stage(void *);
static int   _pipeline_send(pipeline_stage_t *stage, void *data, int exit);


pipeline_t *
pipeline_create(int stages, pipeline_stage_process_pf *processes)
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

    stage->msg.data = NULL;
    stage->msg.exit = 0;
    stage->data_ready = 0;
    stage->index = i;
    pl->head = stage;
    pl->stages++;

    if (i == stages) {
      stage->proc = processes[stages-1];
      pl->tail = stage;
      stage->next = NULL;
      res = pthread_create(&stage->thread, NULL,
                           _pipeline_stage_tail, (void *) pl);

    } else {
      stage->proc = processes[i-1];
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

  return _pipeline_send(pl->head, data, 0);
}


kerrno_t
pipeline_wait(pipeline_t *pl, int destroy)
{
  kerrno_t res;

  /* force invoking all stages, and then exit */
  if (destroy == 1) {
    pthread_mutex_lock(&pl->mutex);
    pl->active++;
    pthread_mutex_unlock(&pl->mutex);

    _pipeline_send(pl->head, NULL, 1);
  }

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

  if (pthread_mutex_lock(&stage->mutex) != 0)
    kerror("state_tail lock error");

  while (KTRUE) {
    while (stage->data_ready == 0) {
      if (pthread_cond_wait(&stage->avail, &stage->mutex)) {
        pthread_mutex_unlock(&stage->mutex);
        kerror("stage_tail cond wait error");
      }
    }

    if (stage->msg.exit == 1) {
      pthread_mutex_unlock(&stage->mutex);
      break;
    }

    if (stage->proc(stage->index, &stage->msg.data) != KSUCCESS)
      kinfo("stage proc error");

    pthread_mutex_unlock(&stage->mutex);
    pthread_mutex_lock(&pl->mutex);
    if ((--pl->active) == 0) {
      pthread_cond_signal(&pl->finished);
    }
    pthread_mutex_unlock(&pl->mutex);

    pthread_mutex_lock(&stage->mutex);
    stage->data_ready = 0;
    pthread_cond_signal(&stage->ready);
  }

  pthread_mutex_lock(&pl->mutex);
  kinfo("the stage[thread-%d] exit", stage->index);
  --pl->active;
  pthread_cond_signal(&pl->finished);
  pthread_mutex_unlock(&pl->mutex);
  return (void *) KSUCCESS;
}


static void *
_pipeline_stage(void *arg)
{
  pipeline_stage_t *stage = (pipeline_stage_t *) arg;
  kerrno_t          ret = KSUCCESS;

  if (pthread_mutex_lock(&stage->mutex)) {
    kinfo("pipeline stage lock mutex error");
    return (void *) KERROR;
  }

  while (KTRUE) {
    while (stage->data_ready == 0) {
      if (pthread_cond_wait(&stage->avail, &stage->mutex)) {
        kinfo("pipeline stage wait avail cond error");
        ret = KERROR;
        goto DONE;
      }
    }

    /* force invoking the next stage */
    if (stage->msg.exit == 1) {
      pthread_mutex_unlock(&stage->mutex);
      kinfo("stage[thread-%d] exited", stage->index);
      _pipeline_send(stage->next, NULL, 1);
      break;
    }

    if (stage->proc(stage->index, &stage->msg.data) != KSUCCESS)
      kinfo("stage proc error");

    pthread_mutex_unlock(&stage->mutex);
    _pipeline_send(stage->next, stage->msg.data, 0);
    pthread_mutex_lock(&stage->mutex);

    stage->data_ready = 0;
    if (pthread_cond_signal(&stage->ready)) {
      kinfo("pipeline stage signal ready error");
      ret = KERROR;
      break;
    }
  }

DONE:
  if (pthread_mutex_unlock(&stage->mutex))
    kinfo("pipeline stage unlock mutex error");
  return (void *) (long) ret;
}


static kerrno_t
_pipeline_send(pipeline_stage_t *stage, void *data, int exit)
{
  if (pthread_mutex_lock(&stage->mutex) != 0)
    return KERROR;

  while (stage->data_ready) {
    if (pthread_cond_wait(&stage->ready, &stage->mutex) != 0) {
      pthread_mutex_unlock(&stage->mutex);
      return KERROR;
    }
  }

  stage->msg.data = data;
  stage->msg.exit = exit;
  stage->data_ready = 1;
  if (pthread_cond_signal(&stage->avail) != 0) {
    pthread_mutex_unlock(&stage->mutex);
    return KERROR;
  }

  if (pthread_mutex_unlock(&stage->mutex) != 0)
    return KERROR;

  return KSUCCESS;
}

