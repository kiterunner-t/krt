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


typedef struct t_list_s {
  list_t *l;
  long    insert_num;
  long    delete_num;
  long    find_num;
} t_list_t;


static void *_routine_insert(void *arg);
static void *_routine_delete(void *arg);
static void *_routine_find(void *arg);


static long g_notfound_cnt;


int
main(int argc, char **argv)
{
  thread_t **threads;
  t_list_t   tl;
  int        find_thread_num;
  int        i;

  if (argc < 4)
    kerror("Usage: %s <find_thread_num> <insert_num> <delete_num> <find_num>\n", 
           argv[0]);

  find_thread_num = atol(argv[1]);
  tl.insert_num = atol(argv[2]);
  tl.delete_num = atol(argv[3]);
  tl.find_num = atol(argv[4]);

  threads = (thread_t **) malloc(sizeof(thread_t *) * (find_thread_num + 2));
  if (threads == NULL)
    kerror("malloc threads error");

  tl.l = list_new(&g_long_op);
  if (tl.l == NULL)
    kerror("list_new error");

  threads[0] = thread_new(NULL, _routine_insert, (void *) &tl);
  thread_start(threads[0]);

/*
  thread_destroy(threads[0], 1);
  return 0;
*/

  threads[1] = thread_new(NULL, _routine_delete, (void *) &tl);
  if (threads[0]==NULL || threads[1]==NULL)
    kerror("thread_new error");

  for (i = 2; i < find_thread_num+2; ++i) {
    threads[i] = thread_new(NULL, _routine_find, (void *) &tl);
    if (threads[i] == NULL)
      kerror("thread_new error");
  }

  for (i = 0; i < find_thread_num+2; ++i)
    thread_start(threads[i]);

  for (i = 0; i < find_thread_num+2; ++i)
    thread_destroy(threads[i], 1);

  list_destroy(tl.l);

  printf("not found cnt: %ld\n", g_notfound_cnt);
  return EXIT_SUCCESS;
}


static void *
_routine_insert(void *arg)
{
  thread_t *thread = (thread_t *) arg;
  t_list_t *tl = (t_list_t *) thread_arg(thread);
  long      insert_num = tl->insert_num;
  long      t;
  long      i;
  kerrno_t  ret;

  for (i = 0; i < insert_num; ++i) {
    t = random();
    if (t == (long) KITEM_NULL)
      continue;

    ret = list_insert(tl->l, thread, (kitem_t) t);
    if (ret!=KSUCCESS && ret!=KEEXIST)
      kerror("insert error: %ld[%d]\n", t, ret);
  }

  list_print(tl->l, thread);
  return NULL;
}


static void *
_routine_delete(void *arg)
{
  thread_t *thread = (thread_t *) arg;
  t_list_t *tl = (t_list_t *) thread_arg(thread);
  long      delete_num = tl->delete_num;
  long      t;
  long      i;
  kerrno_t  ret;

  for (i = 0; i < delete_num; ++i) {
    t = random();

    ret = list_delete(tl->l, thread, (kitem_t) t);
    if (ret!=KSUCCESS && ret!=KENOTFOUND)
      kerror("delete error: %ld\n", t);
  }

  return NULL;
}


static void *
_routine_find(void *arg)
{
  thread_t *thread = (thread_t *) arg;
  t_list_t *tl = (t_list_t *) thread_arg(thread);
  long      find_num = tl->find_num;
  long      t;
  long      i;
  kitem_t   result;

  for (i = 0; i < find_num; ++i) {
    t = random();

    result = list_find(tl->l, thread, (kitem_t) t);
    if (result == KITEM_NULL)
      g_notfound_cnt++;
  }

  printf("%ld in find routine\n", g_notfound_cnt);
  fflush(stdout);

  return NULL;
}

