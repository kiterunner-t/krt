/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include <pthread.h>
#include "thread.h"
#include "lock.h"


struct lock_s {
  pthread_mutex_t mutex;
};


lock_t *
lock_new()
{
  lock_t *l;

  l = (lock_t *) malloc(sizeof(*l));
  if (l == NULL)
    return NULL;

  if (pthread_mutex_init(&l->mutex, NULL) < 0) {
    free(l);
    return NULL;
  }

  return l;
}


void
lock_destroy(lock_t *l)
{
  pthread_mutex_destroy(&l->mutex);
  free(l);
}


void
lock(lock_t *l, thread_t *t)
{
  (void) t;
  pthread_mutex_lock(&(l->mutex));
}


void
unlock(lock_t *l, thread_t *t)
{
  (void) t;
  pthread_mutex_unlock(&(l->mutex));
}

