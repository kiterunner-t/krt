/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef LOCK_H_
#define LOCK_H_

#include "thread.h"

#define mem_barrier() __asm__ __volatile__("" : : : "memory")


typedef struct lock_s lock_t;


extern lock_t *lock_new();
extern void    lock_destroy(lock_t *lock);

extern void    lock(lock_t *lock, thread_t *thread);
extern void    unlock(lock_t *lock, thread_t *thread);


#endif
