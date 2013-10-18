/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef TREAP_TREE_H_
#define TREAP_TREE_H_

#include "kitem.h"


typedef struct treap_tree_s treap_tree_t;


extern treap_tree_t *treap_tree_init(kitem_op_t *op);
extern void          treap_tree_destroy(treap_tree_t *treap);

extern kerrno_t      treap_tree_insert(treap_tree_t *treap, kitem_t item);
extern kerrno_t      treap_tree_delete(treap_tree_t *treap, kitem_t item);
extern kitem_t       treap_tree_search(treap_tree_t *treap, kkey_t key);
extern void          treap_tree_print(treap_tree_t *treap);


#endif

