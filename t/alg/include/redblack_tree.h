/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef REDBLACK_TREE_H_
#define REDBLACK_TREE_H_

#include <stdlib.h>
#include "kitem.h"


typedef struct redblack_tree_s redblack_tree_t;


extern redblack_tree_t *redblack_tree_init(kitem_op_t *op, kitem_t max);
extern void redblack_tree_destroy(redblack_tree_t *rb);

extern int  redblack_tree_insert(redblack_tree_t *rb, kitem_t item);
extern void redblack_tree_delete(redblack_tree_t *rb, kitem_t item);

extern size_t redblack_tree_size(redblack_tree_t *rb);
extern size_t redblack_tree_rotate_count(redblack_tree_t *rb);
extern void redblack_tree_print(redblack_tree_t *rb);


#endif

