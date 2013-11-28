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
  lock_t      *lock;
  list_node_t *next;
};


struct list_s {
  kitem_op_t  *op;
  list_node_t *head;
  list_node_t *dummy;
};


static void _list_free_node(list_t *l, list_node_t *node);
static list_node_t *_list_new_node(kitem_t item, list_node_t *next);
static kbool_t _list_validate(list_t *l, list_node_t *prev, list_node_t *cur);


list_t *
list_new(kitem_op_t *op)
{
  list_t *l;

  l = (list_t *) malloc(sizeof(*l));
  if (l == NULL)
    return NULL;

  l->dummy = _list_new_node(KITEM_NULL, NULL);
  if (l->dummy == NULL) {
    free(l);
    return NULL;
  }

  l->head = _list_new_node(KITEM_NULL, l->dummy);
  if (l->head == NULL) {
    free(l);
    _list_free_node(l, l->dummy);
    return NULL;
  }

  l->op = op;
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
    _list_free_node(l, node);
  }

  free(l);
}


static void
_list_free_node(list_t *l, list_node_t *node)
{
  lock_destroy(node->lock);
  node->lock = NULL;
  node->next = NULL;
  if (l->op->free != NULL)
    l->op->free(node->item);
  free(node);
}


static list_node_t *
_list_new_node(kitem_t item, list_node_t *next)
{
  list_node_t *node;

  node = (list_node_t *) malloc(sizeof(*node));
  if (node == NULL)
    return NULL;

  node->lock = lock_new();
  if (node->lock == NULL) {
    free(node);
    return NULL;
  }

  node->next = next;
  node->item = item;
  return node;
}


kerrno_t
list_insert(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t  *dummy = l->dummy;
  list_node_t  *prev;
  list_node_t  *cur;
  list_node_t  *node;
  kerrno_t      ret = KSUCCESS;
  long          n;
  kitem_cmp_pt  cmp = l->op->cmp;

  while (1) {
    prev = l->head;
    for (cur = prev->next; cur != dummy; prev = cur, cur = cur->next) {
      n = cmp(cur->item, item);
      if (n >= 0)
        break;
    }
  
    lock(prev->lock, thread);
    lock(cur->lock, thread);
    if (_list_validate(l, prev, cur) == KTRUE) {
      if (n == 0) {
        unlock(cur->lock, thread);
        unlock(prev->lock, thread);
        ret = KEEXIST;

      } else {
        node = _list_new_node(item, cur);
        if (node == NULL)
          ret = KEMEM;
        else
          prev->next = node;
      }

      unlock(cur->lock, thread);
      unlock(prev->lock, thread);
      return ret;
    }

    unlock(cur->lock, thread);
    unlock(prev->lock, thread);
  }
}


kerrno_t
list_delete(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t  *dummy = l->dummy;
  list_node_t  *prev;
  list_node_t  *cur;
  kerrno_t      ret = KSUCCESS;
  long          n;
  kitem_cmp_pt  cmp = l->op->cmp;

  while (1) {
    prev = l->head;
    for (cur = prev->next; cur != dummy;  prev = cur, cur = cur->next) {
      n = cmp(cur->item, item);
      if (n >= 0)
        break;
    }
  
    lock(prev->lock, thread);
    lock(cur->lock, thread);
    if (_list_validate(l, prev, cur) == KTRUE) {
      if (n == 0) {
        prev->next = cur->next;
        _list_free_node(l, cur);

      } else {
        ret = KENOTFOUND;
      }

      unlock(cur->lock, thread);
      unlock(prev->lock, thread);
      return ret;
    }

    unlock(cur->lock, thread);
    unlock(prev->lock, thread);
  }
}


kitem_t
list_find(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t  *dummy = l->dummy;
  list_node_t  *prev;
  list_node_t  *cur;
  kitem_t       ret = KITEM_NULL;
  long          n;
  kitem_cmp_pt  cmp = l->op->cmp;

  while (1) {
    prev = l->head;
    for (cur = prev->next; cur != dummy;  prev = cur, cur = cur->next) {
      n = cmp(cur->item, item);
      if (n >= 0)
        break;
    }
  
    lock(prev->lock, thread);
    lock(cur->lock, thread);
    if (_list_validate(l, prev, cur) == KTRUE) {
      if (n == 0)
        ret = cur->item;

      unlock(cur->lock, thread);
      unlock(prev->lock, thread);
      return ret;
    }

    unlock(cur->lock, thread);
    unlock(prev->lock, thread);
  }
}


void
list_print(list_t *l, thread_t *thread)
{
  list_node_t    *cur;
  list_node_t    *prev;
  list_node_t    *head;
  list_node_t    *dummy;
  kitem_print_pt  print;

  dummy = l->dummy;
  head = l->head;
  print = l->op->print;

  prev = head;
  lock(prev->lock, thread);
  cur = prev->next;
  while (1) {
    lock(cur->lock, thread);
    if (cur == dummy) 
      break;

    print(cur->item);
    printf("\n");

    unlock(prev->lock, thread);
    prev = cur;
    cur = cur->next;
  }

  printf("\n\n");
  unlock(cur->lock, thread);
  unlock(prev->lock, thread);
}


static kbool_t
_list_validate(list_t *l, list_node_t *prev, list_node_t *cur)
{
  list_node_t *p;
  list_node_t *dummy = l->dummy;

  for (p = l->head; p != dummy; p = p->next) {
    if (p == prev)
      return (kbool_t) (p->next == cur);
  }

  return KFALSE;
}

