/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include "thread.h"
#include "lock.h"


static void *_subroutine(void *arg);


static int     g_shared;
static lock_t *g_lock_shared;


int
main()
{
  thread_t *t1;
  thread_t *t2;

  g_lock_shared = lock_new();
  if (g_lock_shared == NULL)
    return 1;

  t1 = thread_new(NULL, _subroutine, NULL);
  t2 = thread_new(NULL, _subroutine, NULL);
  if (t1==NULL || t2==NULL) {
    lock_destroy(g_lock_shared);
    thread_destroy(t1, 0);
    thread_destroy(t2, 0);
    return 1;
  }

  thread_start(t1);
  thread_start(t2);
  thread_destroy(t1, 1);
  thread_destroy(t2, 1);
  lock_destroy(g_lock_shared);

  printf("shared is: %d\n", g_shared);
  return 0;
}


static void *
_subroutine(void *arg)
{
  thread_t *thread = (thread_t *) arg;
  int       i;

  for (i = 0; i < 500000; ++i) {
    lock(g_lock_shared, thread);
    g_shared++;
    unlock(g_lock_shared, thread);
  }

  return (void *) thread;
}

