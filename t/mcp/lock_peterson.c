/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include "thread.h"
#include "lock.h"

#ifdef KUSE_GCC_ATOMIC
# include "katomic.h"
#endif


struct lock_s {
  int flag[2];
  int victim;
};


lock_t *
lock_new()
{
  lock_t *l;

  l = (lock_t *) malloc(sizeof(*l));
  if (l == NULL)
    return NULL;

  l->flag[0] = 0;
  l->flag[1] = 0;
  l->victim = 0;
  return l;
}


void
lock_destroy(lock_t *l)
{
  if (l == NULL)
    return ;

  free(l);
}


void
lock(lock_t *l, thread_t *thread)
{
  int i;
  int j;

  i = thread_id(thread);
  j = 1 - i;
  l->flag[i] = 1;
  l->victim = i;

#ifdef KUSE_GCC_ATOMIC
  kbarrier();
#endif

  while (l->flag[j] && l->victim==i)
    ; // spin
}


void
unlock(lock_t *l, thread_t *thread)
{
  int i;

  i = thread_id(thread);
  l->flag[i] = 0;
}

