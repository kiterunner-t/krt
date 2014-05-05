/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <stdlib.h>
#include <pthread.h>


typedef void *(*thread_start_pt)(void *);

typedef struct thread_s thread_t;


extern thread_t *thread_new(pthread_attr_t *attr,
                            thread_start_pt start,
                            void *arg);
extern void      thread_destroy(thread_t *thread, int join);
extern void      thread_start(thread_t *thread);

extern int       thread_id(thread_t *thread);
extern void     *thread_arg(thread_t *thread);

extern void     *thread_local_new(thread_t *thread, int *local_id, int size);
extern void      thread_local_destroy(thread_t *thread, int local_id);
extern void     *thread_local_get(thread_t *thread, int local_id);


#endif

