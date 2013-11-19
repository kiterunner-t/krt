/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lock.h"
#include "thread.h"

/* #define CACHELINE 64 sysconf(_SC_LEVEL1_DCACHE_LINESIZE) */

typedef struct cacheline_s cacheline_t;


struct cacheline_s {
  int  num;

#ifdef CACHELINE
  char pad[CACHELINE-sizeof(int)];
#endif
};

struct lock_s {
  int         tail;
  int         size;
  cacheline_t flag[1];
};


lock_t *
lock_new1(int size)
{
  lock_t *l;

  l = (lock_t *) malloc(sizeof(*l) + size * sizeof(cacheline_t));
  if (l == NULL)
    return NULL;

  l->size = size;
  l->tail = 0;
  memset(l->flag, 0, size * sizeof(cacheline_t));
  l->flag[0].num = 1;
  return l;
}


void
lock_destroy(lock_t *l)
{
  assert(l != NULL);
  free(l);
}


void
lock(lock_t *l, thread_t *t)
{
  int slot;

  slot = __sync_fetch_and_add(&(l->tail), 1) % l->size;
  thread_id_set(t, slot);

  while (l->flag[slot].num == 0)
    ;
}


void
unlock(lock_t *l, thread_t *t)
{
  int slot;

  slot = thread_id(t);
  l->flag[slot].num = 0;
  l->flag[(slot+1)%(l->size)].num = 1;
}

