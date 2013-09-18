/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "skip_list_deterministic.h"


typedef struct dskip_list_s dskip_list_t;
typedef struct dskip_list_node_s dskip_list_node_t;


struct dskip_list_s {
  dskip_list_node_t *head;
  dskip_list_node_t *tail;
  dskip_list_node_t *bottom;
  size_t             size;
  item_op_t         *op;
  void              *max;
};


struct dskip_list_node_s {
  dskip_list_node_t *right;
  dskip_list_node_t *down;
  void              *item;
};


dskip_list_t *
dskip_list_init(item_op_t *op, void *max)
{
  dskip_list_node_t *h;
  dskip_list_t *dsl;
  dskip_list_node_t *b;
  dskip_list_node_t *t;

  dsl = (dskip_list_t *) malloc(sizeof(*dsl));
  if (dsl == NULL)
    return NULL;

  h = (dskip_list_node_t *) malloc(sizeof(*h));
  if (h == NULL) {
    free(dsl);
    return NULL;
  }

  b = (dskip_list_node_t *) malloc(sizeof(*b) * 2);
  if (b == NULL) {
    free(dsl);
    free(h);
    return NULL;
  }

  t = b + 1;
  h->right = t;
  h->down = b;
  h->item = max;
  t->right = t;
  t->down = NULL;
  t->item = max;
  b->right = b;
  b->down = b;
  b->item = 0;

  dsl->head = h;
  dsl->tail = t;
  dsl->bottom = b;

  dsl->op = op;
  dsl->size = 0;
  dsl->max = max;
  return dsl;
}


void
dskip_list_destroy(dskip_list_t *dsl)
{
  dskip_list_node_t *level;
  dskip_list_node_t *current;

  if (dsl == NULL)
    return ;

  if (dsl->op->free != NULL) {
    for (level = dsl->head; level != dsl->bottom; level = level->down) {
      for (current = level; current != dsl->tail; current = current->right) 
        (dsl->op->free)(current->item);
    }
  }

  free(dsl->bottom);
  free(dsl);
}


int
dskip_list_find(dskip_list_t *dsl, void *item, void **result)
{
  dskip_list_node_t *current;
  item_cmp_pt        cmp;
  long               n;

  if (cmp(item, dsl->max) == 0)
    return -1;

  current = dsl->head;
  while (current != dsl->bottom) {
    n = cmp(item, current->item);
    if (n == 0) {
      *result = current->item;
      return 0;
    } else if (n > 0) {
      current = current->right;
    } else {
      current = current->down;
    }
  }

  return -1;
}


static dskip_list_node_t *
_dskip_list_node_new(void *item, dskip_list_node_t *r, dskip_list_node_t *d)
{
  dskip_list_node_t *t;

  t = (dskip_list_node_t *) malloc(sizeof(*t));
  if (t == NULL)
    return NULL;

  t->item = item;
  t->right = r;
  t->down = d;
  return t;
}


int
dskip_list_insert(dskip_list_t *dsl, void *item)
{
  dskip_list_node_t *current;
  dskip_list_node_t *t;
  item_cmp_pt        cmp = dsl->op->cmp;

  current = dsl->head;
  dsl->bottom->item = item;

  while (current != dsl->bottom) {
    while (cmp(item, current->item) > 0)
      current = current->right;

    if (current->down==dsl->bottom && cmp(item, current->item)==0) 
      return 1;

    if (cmp(current->item, current->down->right->right->item) > 0) {
      t = _dskip_list_node_new(current->item, current->right,
                               current->down->right->right);
      if (t == NULL)
        return -1;

      current->right = t;
      current->item = current->down->right->item;
    }

    current = current->down;
  }

  if (dsl->head->right != dsl->tail) {
    t = _dskip_list_node_new(dsl->max, dsl->tail, dsl->head);
    if (t == NULL)
      return -1;
    dsl->head = t;
  }

  dsl->size++;
  return 0;
}


void
dskip_list_delete(dskip_list_t *dsl, void *item)
{
  printf("not support by now\n");
}


void
dskip_list_print(dskip_list_t *dsl)
{
  dskip_list_node_t  *level;
  dskip_list_node_t  *current;
  item_print_pt       print = dsl->op->print;
  int                 i = 1;

  if (dsl->size == 0) {
    printf("empty\n\n");
    return ;
  }

  for (level = dsl->head; level != dsl->bottom; 
       level = level->down) {

    printf("level %d: ", i++);
    for (current = level; current != dsl->tail; 
         current = current->right) {

      if (current->right != dsl->tail)
        print(current->item);
      else
        printf("h");

      if (current->down != dsl->bottom) {
        printf("[");
        print(current->down->item);
        printf("]");
      }
      printf(" ");
    }

    printf("\n");
  }

  printf("\n");
}

