/**
 * Copyleft (C) KRT, 2012 2013 by kiterunner_t
 */

#ifndef ST_H_
#define ST_H_

#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"


typedef void (*st_visit_pt)(kitem_t item);


extern kerrno_t st_init(kitem_op_t *op);
extern void     st_destroy(void);
extern size_t   st_size(void);
extern kerrno_t st_insert(kitem_t item);
extern kitem_t  st_search(kkey_t key);
extern void     st_delete(kitem_t item);
extern kitem_t  st_select(size_t num);
extern void     st_sort(st_visit_pt visit);


#endif

