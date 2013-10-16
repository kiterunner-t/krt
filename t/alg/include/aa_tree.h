/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef AA_TREE_H_
#define AA_TREE_H_

#include <stdlib.h>


typedef struct aa_tree_s aa_tree_t;


extern aa_tree_t *aa_tree_init(item_op_t *op);
extern void       aa_tree_destroy(aa_tree_t *aa);

extern errno_t    aa_tree_insert(aa_tree_t *aa, void *item);
extern errno_t    aa_tree_delete(aa_tree_t *aa, void *item);
extern void       aa_tree_print(const aa_tree_t *aa);
extern size_t     aa_tree_size(const aa_tree_t *aa);


#endif

