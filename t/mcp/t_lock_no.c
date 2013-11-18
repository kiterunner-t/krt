/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "thread.h"


typedef struct thread_local_s thread_local_t;

struct thread_local_s {
  long cnt;
  long max_cnt;
};


static void *_subroutine(void *arg);


static long    g_shared;


int
main(int argc, char **argv)
{
  thread_t       **threads;
  thread_local_t  *thread_locals;
  int              i;
  int              thread_num;
  long             shared_max;

  if (argc != 3)
    kerror("Usage: %s <thread_num> <shared_max>", argv[0]);

  thread_num = atoi(argv[1]);
  shared_max = atol(argv[2]);
  if (thread_num<2 || shared_max<10000)
    kerror("please increase the <thread_num> or <shared_max>");

  threads = (thread_t **) malloc(sizeof(thread_t *) * thread_num);
  if (threads == NULL)
    kerror("malloc threads error");

  thread_locals = (thread_local_t *) malloc(sizeof(thread_local_t) * thread_num);
  if (thread_locals == NULL) 
    kerror("malloc thread_locals error");

  for (i = 0; i < thread_num; ++i) {
    thread_locals[i].cnt = 0;
    thread_locals[i].max_cnt = shared_max;

    threads[i] = thread_new(NULL, _subroutine, (void *) &thread_locals[i]);
    if (threads[i] == NULL)
      kerror("thread_new error");
  }

  for (i = 0; i < thread_num; ++i)
    thread_start(threads[i]);

  for (i = 0; i < thread_num; ++i)
    thread_destroy(threads[i], 1);

  for (i = 0; i < thread_num; ++i)
    g_shared += thread_locals[i].cnt;
  printf("shared is: %ld\n", g_shared);

  free(threads);
  free(thread_locals);
  return EXIT_SUCCESS;
}


static void *
_subroutine(void *arg)
{
  thread_t       *thread = (thread_t *) arg;
  long            i;
  thread_local_t *tl;
  long            thread_num;

  tl = (thread_local_t *) thread_arg(thread);
  thread_num = tl->max_cnt;
  for (i = 0; i < thread_num; ++i)
    tl->cnt++;

  return (void *) thread;
}

