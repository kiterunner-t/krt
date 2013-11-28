/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"
#include "lock.h"
#include "thread.h"
#include "list.h"


typedef struct list_node_s list_node_t;


struct list_node_s {
  kitem_t      item;
  list_node_t *next;
};


struct list_s {
  kitem_op_t  *op;
  lock_t      *lock;
  list_node_t *head;
};


list_t *
list_new(kitem_op_t *op)
{
  list_t *l;

  l = (list_t *) malloc(sizeof(*l));
  if (l == NULL)
    return NULL;

  l->lock = lock_new();
  if (l->lock == NULL) {
    free(l);
    return NULL;
  }

  l->op = op;
  l->head = NULL;
  return l;
}


void
list_destroy(list_t *l)
{
  list_node_t *node;

  assert(l != NULL);

  while (l->head != NULL) {
    node = l->head;
    l->head = node->next;
    node->next = NULL;
    free(node);
  }

  lock_destroy(l->lock);
  free(l);
}


kerrno_t
list_insert(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t   *node;
  list_node_t   *cur;
  list_node_t  **prev;
  kitem_cmp_pt   cmp;
  long           n;

  cmp = l->op->cmp;
  lock(l->lock, thread);
  cur = l->head;
  prev = &l->head;
  while (cur != NULL) {
    n = cmp(item, cur->item);
    if (n == 0) {
      unlock(l->lock, thread);
      return KEEXIST;

    } else if (n > 0) {
      break;
    }

    prev = &cur->next;
    cur = cur->next;
  }

  node = (list_node_t *) malloc(sizeof(*node));
  if (node == NULL) {
    unlock(l->lock, thread);
    return KEMEM;
  }

  node->item = item;
  node->next = cur;
  *prev = node;
  unlock(l->lock, thread);
  return KSUCCESS;
}
  

kerrno_t
list_delete(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t  *cur;
  list_node_t  *prev;
  kitem_cmp_pt  cmp;
  long          n;
  kerrno_t      ret = KSUCCESS;

  cmp = l->op->cmp;
  lock(l->lock, thread);
  cur = l->head;
  prev = l->head;
  while (cur != NULL) {
    n = cmp(item, cur->item);
    if (n == 0) {
      prev->next = cur->next;
      cur->next = NULL;
      free(cur);
      ret = KSUCCESS;
      break;

    } else if (n > 0) {
      ret = KENOTFOUND;
      break;
    }

    prev = cur;
    cur = cur->next;
  }

  unlock(l->lock, thread);
  return ret;
}


kitem_t
list_find(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t  *cur;
  kitem_cmp_pt  cmp;
  long          n;

  cmp = l->op->cmp;
  lock(l->lock, thread);
  cur = l->head;
  while (cur != NULL) {
    n = cmp(item, cur->item);
    if (n == 0) {
      unlock(l->lock, thread);
      return cur->item;

    } else if (n > 0) {
      break;
    }

    cur = cur->next;
  }

  unlock(l->lock, thread);
  return KITEM_NULL;
}


void
list_print(list_t *l, thread_t *thread)
{
  list_node_t    *cur;
  kitem_print_pt  print = l->op->print;
  int             cnt = 0;

  lock(l->lock, thread);
  for (cur = l->head; cur != NULL; cur = cur->next) {
    print(cur->item);
    printf("\n");
    cnt++;
  }

  printf("this is list %d nodes\n", cnt);
  unlock(l->lock, thread);
}

