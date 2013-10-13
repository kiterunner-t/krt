/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef SPLAY_TREE_T_
#define SPLAY_TREE_T_


typedef struct splay_tree_s splay_tree_t;


splay_tree_t *splay_tree_init(item_op_t *op);
void          splay_tree_destroy(splay_tree_t *splay);
int           splay_tree_insert(splay_tree_t *splay, void *item);

void          splay_tree_print(const splay_tree_t *splay);


#endif

