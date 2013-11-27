/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include "lock.h"


struct lock_s {
  volatile int state;
};


lock_t *
lock_new()
{
  lock_t *lock;

  lock = (lock_t *) malloc(sizeof(*lock));
  lock->state = 0;
  return lock;
}


void
lock_destroy(lock_t *lock)
{
  free(lock);
}


void
lock(lock_t *lock, thread_t *thread)
{
  while (__sync_val_compare_and_swap(&lock->state, 0, 1))
    ;
}


void
unlock(lock_t *lock, thread_t *thread)
{
  lock->state = 0;
}

