/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include "kitem.h"
#include "kmisc.h"


typedef struct avl_tree_s avl_tree_t;


extern avl_tree_t *avl_tree_init(kitem_op_t *op);
extern void        avl_tree_destroy(avl_tree_t *avl);

extern int         avl_tree_height(const avl_tree_t *avl);
extern kerrno_t    avl_tree_insert(avl_tree_t *avl, kitem_t item);
extern kerrno_t    avl_tree_delete(avl_tree_t *avl, kitem_t item);
extern kitem_t     avl_tree_search(avl_tree_t *avl, kkey_t key);
extern void        avl_tree_print(avl_tree_t *avl);


#endif

