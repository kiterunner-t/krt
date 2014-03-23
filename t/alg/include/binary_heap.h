/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef BINARY_HEAP_H_
#define BINARY_HEAP_H_

#include "kmisc.h"


typedef struct binary_heap_s binary_heap_t;


extern binary_heap_t *binary_heap_init(kitem_op_t *op, int max_size);
extern void           binary_heap_destroy(binary_heap_t *bh);

extern kbool_t binary_heap_is_empty(binary_heap_t *bh);
extern void    binary_heap_print(binary_heap_t *bh);

extern kerrno_t binary_heap_insert(binary_heap_t *bh, kitem_t n);
extern kerrno_t binary_heap_delete_min(binary_heap_t *bh);


#endif

