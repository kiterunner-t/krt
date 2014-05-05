/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include "thread.h"


#define THREAD_LOCAL_STACK_SIZE          (sizeof(long) * 8)


/*
 * the max size of local stack is equal to sizeof(long) * 8, and
 * local_type_map represent the type of the slot of stack.
 *
 * if the size of local stack slot is not great than sizeof(long),
 * the value of the slot will be stored at the slot, or I will be malloc
 * a new memory for storing the value.
 *
 * Actually, you should use pthread_key_create/pthread_setspecific/
 * pthread_getspecific for thread local variables.
 */
struct thread_s {
  pthread_t         pthread;
  int               id;
  pthread_attr_t   *attr;
  void            **local_stack;
  long              local_type_map;
  int               local_stack_used_map;
  void             *arg;

  thread_start_pt   start;
};


thread_t *
thread_new(pthread_attr_t *attr, thread_start_pt start, void *arg)
{
  thread_t   *thread;
  static int  id = 0;
  int         tlen;

  tlen = sizeof(*thread) + THREAD_LOCAL_STACK_SIZE * sizeof(void **);
  thread = (thread_t *) malloc(tlen);
  if (thread == NULL)
    return NULL;

  memset(thread, 0, tlen);
  thread->id = id++;
  thread->arg = arg;
  thread->attr = attr;
  thread->start = start;
  thread->local_stack = (void **) (thread + 1);

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


void *
thread_arg(thread_t *thread)
{
  assert(thread != NULL);
  return thread->arg;
}


void *
thread_local_new(thread_t *thread, int *local_id, int size)
{
  void *t;
  int   id = -1;
  int   i;

  assert(thread != NULL);
  assert(local_id != NULL);

  *local_id = -1;

  if (size < 1)
    return NULL;

  for (i = 0; i < THREAD_LOCAL_STACK_SIZE; ++i) {
    if ((thread->local_stack_used_map & (1 << i)) == 0) {
      id = i;
      break;
    }
  }

  if (id == -1)
    return NULL;

  if (size <= (int) sizeof(void *)) {
    thread->local_type_map |= (1 << id);
    thread->local_stack_used_map |= (1 << id);
    *local_id = id;
    return &thread->local_stack[id];
  }

  t = malloc(size);
  if (t == NULL)
    return t;

  thread->local_stack_used_map |= (1 << id);
  *local_id = id;
  thread->local_stack[id] = t;
  return t;
}


void
thread_local_destroy(thread_t *thread, int local_id)
{
  long type_map;

  if (local_id<0 || local_id>THREAD_LOCAL_STACK_SIZE)
    return ;

  thread->local_stack_used_map &= ~(1 << local_id);
  type_map = thread->local_type_map;
  if ((type_map & (1 << local_id)) == 0)
    free(thread->local_stack[local_id]);
}


void *
thread_local_get(thread_t *thread, int local_id)
{
  long type_map;

  assert(thread != NULL);

  if (local_id<0 || local_id>THREAD_LOCAL_STACK_SIZE)
    return NULL;
  if ((thread->local_stack_used_map & (1 << local_id)) == 0)
    return NULL;

  type_map = thread->local_type_map;
  if ((type_map & (1 << local_id)) == 0)
    return thread->local_stack[local_id];

  return &thread->local_stack[local_id];
}

