/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "kmisc.h"
#include "kitem.h"
#include "kitem_long.h"
#include "lock.h"
#include "thread.h"
#include "list.h"


static void *_routine_insert0(void *arg);
static void *_routine_insert1(void *arg);


int
main(int argc, char **argv)
{
  thread_t *threads[2];
  list_t   *l;

  l = list_new(&g_long_op);
  threads[0] = thread_new(NULL, _routine_insert0, (void *) l);
  threads[1] = thread_new(NULL, _routine_insert1, (void *) l);
  thread_start(threads[0]);
  thread_start(threads[1]);
  thread_destroy(threads[0], 1);
  thread_destroy(threads[1], 1);

  list_destroy(l);
  return EXIT_SUCCESS;
}


static void *
_routine_insert0(void *arg)
{
  thread_t *thread = (thread_t *) arg;
  list_t   *l = (list_t *) thread_arg(thread);
  long      i;
  kerrno_t  ret;
  long      t[] = {1, 3, 5, 7, 11, 9, 15, 17};

  for (i = 0; i < sizeof(t)/sizeof(long); ++i) {
    ret = list_insert(l, thread, (kitem_t) t[i]);
    if (ret!=KSUCCESS && ret!=KEEXIST)
      kerror("insert error: %ld[%d]\n", t[i], ret);
  }

  list_print(l, thread);
  return NULL;
}


static void *
_routine_insert1(void *arg)
{
  thread_t *thread = (thread_t *) arg;
  list_t   *l = (list_t *) thread_arg(thread);
  long      i;
  kerrno_t  ret;
  kitem_t   result;
  long      t[] = {16, 14, 10, 12, 8, 6, 4, 2};

  for (i = 0; i < sizeof(t)/sizeof(long); ++i) {
    ret = list_insert(l, thread, (kitem_t) t[i]);
    if (ret!=KSUCCESS && ret!=KEEXIST)
      kerror("insert error: %ld[%d]\n", t[i], ret);

    result = list_find(l, thread, (kitem_t) t[i]);
    if ((long) result != t[i])
      kerror("find %ld error\n", t[i]);

    list_delete(l, thread, (kitem_t) t[i]);

    result = list_find(l, thread, (kitem_t) t[i]);
    if ((long) result != (long) KITEM_NULL)
      kerror("delete %ld[%ld] error\n", t[i], (long) result);
  }

  list_print(l, thread);
  return NULL;
}

