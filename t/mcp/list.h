/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef LIST_H_
#define LIST_H_

#include "kmisc.h"
#include "kitem.h"
#include "thread.h"


typedef struct list_s list_t;


extern list_t   *list_new(kitem_op_t *op);
extern void      list_destroy(list_t *l);
extern kerrno_t  list_insert(list_t *l, thread_t *thread, kitem_t item);
extern kerrno_t  list_delete(list_t *l, thread_t *thread, kitem_t item);
extern kitem_t   list_find(list_t *l, thread_t *thread, kitem_t item);


#endif

