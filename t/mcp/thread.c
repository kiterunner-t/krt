/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include "thread.h"


struct thread_s {
  int              id;
  void            *local;
  pthread_t        pthread;
  pthread_attr_t  *attr;
  thread_start_pt  start;
  void            *arg;
};


thread_t *
thread_new(pthread_attr_t *attr, thread_start_pt start, void *arg)
{
  thread_t   *thread;
  static int  id = 0;

  thread = (thread_t *) malloc(sizeof(*thread));
  if (thread == NULL)
    return NULL;

  thread->id = id++;
  thread->local = NULL;
  thread->arg = arg;
  thread->attr = attr;
  thread->start = start;
  return thread;
}


void
thread_start(thread_t *thread)
{
  assert(thread != NULL);
  (void) pthread_create(&thread->pthread, thread->attr, 
                        thread->start, (void *) thread);
}


void
thread_destroy(thread_t *thread, int join)
{
  if (thread == NULL)
    return ;

  if (join)
    pthread_join(thread->pthread, NULL);
  free(thread);
}


int
thread_id(thread_t *thread)
{
  assert(thread != NULL);
  return thread->id;
}


void
thread_id_set(thread_t *thread, int id)
{
  assert(thread != NULL);
  thread->id = id;
}


void *
thread_arg(thread_t *thread)
{
  assert(thread != NULL);
  return thread->arg;
}


void *
thread_local(thread_t *thread, size_t size)
{
  if (thread->local == NULL) {
    thread->local = malloc(size);
    if (thread->local == NULL)
      return NULL;
  }

  return thread->local;
}

