/**
 * Copyleft (C) KRT, 2012 2013 by kiterunner_t
 */

#ifndef ST_BST_H_
#define ST_BST_H_

#include <stdlib.h>
#include "kitem.h"


typedef struct st_bst_node_s st_bst_node_t;

struct st_bst_node_s {
  kitem_t        item;
  size_t         size;
  st_bst_node_t *left;
  st_bst_node_t *right;
};


#endif

