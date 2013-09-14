/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef REDBLACK_TREE_H_
#define REDBLACK_TREE_H_

#include <stdlib.h>
#include "item.h"


typedef struct redblack_tree_s redblack_tree_t;


redblack_tree_t *redblack_tree_init(item_op_t *op, void *max);
void redblack_tree_destroy(redblack_tree_t *rb);

int  redblack_tree_insert(redblack_tree_t *rb, void *item);
void redblack_tree_delete(redblack_tree_t *rb, void *item);

size_t redblack_tree_size(redblack_tree_t *rb);
size_t redblack_tree_rotate_count(redblack_tree_t *rb);
void redblack_tree_print(redblack_tree_t *rb);


#endif

