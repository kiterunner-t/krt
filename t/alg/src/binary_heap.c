/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"
#include "binary_heap.h"


struct binary_heap_s {
  int         max_size;
  int         used;
  kitem_op_t *op;
  kitem_t     items[1];
};


binary_heap_t *
binary_heap_init(kitem_op_t *op, int max_size)
{
  binary_heap_t *bh;

  bh = (binary_heap_t *) malloc(sizeof(*bh) + sizeof(void *) * max_size);
  if (bh == NULL)
    return NULL;

  bh->op = op;
  bh->max_size = max_size;
  bh->used = 0;
  return bh;
}


void
binary_heap_destroy(binary_heap_t *bh)
{
  kitem_free_pt item_free;
  int           i;

  if (bh == NULL)
    return ;

  item_free = bh->op->free;
  if (item_free != NULL) {
    for (i = 1; i < bh->used; ++i)
      item_free(bh->items[i]);
  }

  free(bh);
}


kbool_t
binary_heap_is_empty(binary_heap_t *bh)
{
  return (bh->used==0) ? KTRUE : KFALSE;
}


kerrno_t
binary_heap_insert(binary_heap_t *bh, kitem_t n)
{
  int           hole;
  int           p;
  kitem_t      *items = bh->items;
  kitem_cmp_pt  cmp;

  if (bh->used == bh->max_size)
    return KEEXCEED;

  cmp = bh->op->cmp;
  hole = ++bh->used;
  while (hole > 1) {
    p = hole / 2;
    if (cmp(n, items[p]) > 0)
      break;

    items[hole] = items[p];
    hole = p;
  }

  items[hole] = n;
  return KSUCCESS;
}


kerrno_t
binary_heap_delete_min(binary_heap_t *bh)
{
  int           hole;
  kitem_t       t;
  int           child;
  kitem_t      *items = bh->items;
  kitem_cmp_pt  cmp;

  if (binary_heap_is_empty(bh))
    return KEEMPTY;

  if (bh->op->free != NULL)
    bh->op->free(bh->items[1]);

  cmp = bh->op->cmp;
  t = items[bh->used--];
  hole = 1;
  for (child = hole * 2; child <= bh->used; child = hole * 2) {
    if (child == bh->used)
      break;

    if (cmp(items[child+1], items[child]) < 0)
      child++;

    items[hole] = items[child];
    hole = child;
  }

  items[hole] = t;
  return KSUCCESS;
}


void
binary_heap_print(binary_heap_t *bh)
{
  int            i;
  kitem_print_pt print;

  if (bh->used == 0) {
    printf("binary heap is empty\n");
    return ;
  }

  print = bh->op->print;
  for (i = 1; i <= bh->used; ++i) {
    print(bh->items[i]);
    printf("\n");
  }

  printf("\n");
}

