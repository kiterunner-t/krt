/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef LOCK_H_
#define LOCK_H_

#include "thread.h"


typedef struct lock_s lock_t;


extern lock_t *lock_new();
extern lock_t *lock_new1(int capacity); /* for alock */
extern void    lock_destroy(lock_t *lock);

extern void    lock(lock_t *lock, thread_t *thread);
extern void    unlock(lock_t *lock, thread_t *thread);


#endif

