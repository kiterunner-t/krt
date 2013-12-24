/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KD_TREE_H_
#define KD_TREE_H_

#include "kitem.h"
#include "kmisc.h"


enum {
  KD_DISENABLE = 0,
  KD_ENABLE
};


typedef struct kd_tree_s    kd_tree_t;
typedef struct kd_item_s    kd_item_t;


struct kd_item_s {
  kitem_t k1;
  kitem_t k2;
};


extern kd_tree_t *kd_tree_init(kitem_op_t *op1, kitem_op_t *op2);
extern void       kd_tree_destroy(kd_tree_t *kd);
extern kerrno_t   kd_tree_insert(kd_tree_t *kd, kd_item_t *item);
extern kerrno_t   kd_tree_delete(kd_tree_t *kd, kd_item_t *item);
extern kerrno_t   kd_tree_print(kd_tree_t *kd, kd_item_t *low, kd_item_t *high);


#endif

