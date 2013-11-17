/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "thread.h"
#include "lock.h"


static void *_subroutine(void *arg);


static long    g_shared;
static lock_t *g_lock_shared;


int
main(int argc, char **argv)
{
  thread_t **threads;
  int        i;
  int        thread_num;
  long       shared_max;

  if (argc != 3)
    kerror("Usage: %s <thread_num> <shared_max>", argv[0]);

  thread_num = atoi(argv[1]);
  shared_max = atol(argv[2]);
  if (thread_num<2 || shared_max<10000)
    kerror("please increase the <thread_num> or <shared_max>");

  threads = (thread_t **) malloc(sizeof(thread_t *) * thread_num);
  if (threads == NULL)
    kerror("malloc threads error");

  g_lock_shared = lock_new();
  if (g_lock_shared == NULL) {
    free(threads);
    kerror("lock_new error");
  }

  for (i = 0; i < thread_num; ++i) {
    threads[i] = thread_new(NULL, _subroutine, (void *) shared_max);
    if (threads[i] == NULL)
      kerror("thread_new error");
  }

  for (i = 0; i < thread_num; ++i)
    thread_start(threads[i]);

  for (i = 0; i < thread_num; ++i)
    thread_destroy(threads[i], 1);
  lock_destroy(g_lock_shared);

  printf("shared is: %ld\n", g_shared);
  return EXIT_SUCCESS;
}


static void *
_subroutine(void *arg)
{
  thread_t *thread = (thread_t *) arg;
  long      i;
  long      thread_num;

  thread_num = (long) thread_arg(thread);
  for (i = 0; i < thread_num; ++i) {
    lock(g_lock_shared, thread);
    g_shared++;
    unlock(g_lock_shared, thread);
  }

  return (void *) thread;
}

