/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KITEM_H_
#define KITEM_H_


#ifndef KITEM_NULL
# define KITEM_NULL       NULL
#endif


typedef void             *kitem_t;
typedef void             *kkey_t;
typedef struct item_op_s  kitem_op_t;


typedef long (*kitem_cmp_pt)(kitem_t item1, kitem_t item2);
typedef long (*kitem_cmp_key1_pt)(kitem_t item, kkey_t key);
typedef void (*kitem_print_pt)(kitem_t item);
typedef void (*kitem_free_pt)(kitem_t item);


struct item_op_s {
  kitem_cmp_pt      cmp;
  kitem_cmp_key1_pt cmp_key1;
  kitem_print_pt    print;
  kitem_free_pt     free;
};


#endif

