/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef SPLAY_TREE_T_
#define SPLAY_TREE_T_

#include "kitem.h"


typedef struct splay_tree_s splay_tree_t;


extern splay_tree_t *splay_tree_init(kitem_op_t *op);
extern void          splay_tree_destroy(splay_tree_t *splay);

extern int           splay_tree_insert(splay_tree_t *splay, kitem_t item);
extern int           splay_tree_delete(splay_tree_t *splay, kitem_t item);
extern kitem_t       splay_tree_search(splay_tree_t *splay, kkey_t key);
extern void          splay_tree_print(const splay_tree_t *splay);


#endif

