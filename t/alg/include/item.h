/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef ITEM_H_
#define ITEM_H_


typedef struct item_op_s item_op_t;


typedef long (*item_cmp_pt)(void *item1, void *item2);
typedef void (*item_print_pt)(void *item);
typedef void (*item_free_pt)(void *item);


struct item_op_s {
  item_cmp_pt   cmp;
  item_print_pt print;
  item_free_pt  free;
};


#endif

