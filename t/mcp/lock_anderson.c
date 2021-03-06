/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "katomic.h"
#include "lock.h"
#include "thread.h"

#ifdef CACHELINE
# include <malloc.h>
#endif

/* #define CACHELINE 64 sysconf(_SC_LEVEL1_DCACHE_LINESIZE) */

typedef union {
  volatile int num;

#ifdef CACHELINE
  char pad[CACHELINE];
#endif
} cacheline_u;


struct lock_s {
  katomic_t    tail;
  long         size;
  cacheline_u *flag;
};


lock_t *
lock_new1(long size)
{
  lock_t *l;

  l = (lock_t *) malloc(sizeof(*l));
  if (l == NULL)
    return NULL;

#ifdef CACHELINE
  l->flag = (cacheline_u *) memalign(CACHELINE, size * sizeof(cacheline_u));
#else
  l->flag = (cacheline_u *) malloc(size * sizeof(cacheline_u));
#endif

  if (l->flag == NULL) {
    free(l);
    return NULL;
  }

  l->size = size;
  l->tail = 0;
  l->flag[0].num = 1;
  return l;
}


void
lock_destroy(lock_t *l)
{
  assert(l != NULL);
  free(l->flag);
  free(l);
}


void
lock(lock_t *l, thread_t *t)
{
  long         slot;
  cacheline_u *c;

  slot = katomic_fetch_add(&(l->tail), 1) % l->size;
  thread_id_set(t, slot);

  c = &(l->flag[slot]);
  while (c->num == 0)
    ;
}


void
unlock(lock_t *l, thread_t *t)
{
  long slot;

  slot = thread_id(t);
  l->flag[slot].num = 0;
  l->flag[(slot+1)%(l->size)].num = 1;
}

