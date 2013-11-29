
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
  list_node_t *free;
  lock_t      *freelock;
};


static void _list_free_node(list_t *l, thread_t *thread, list_node_t *node);
static list_node_t *_list_new_node(kitem_t item, list_node_t *next);
static void _list_destroy_node(list_t *l, list_node_t *node);


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
    _list_destroy_node(l, l->dummy);
    free(l);
    return NULL;
  }

  l->free = NULL;
  l->freelock = lock_new();
  if (l->freelock == NULL) {
    _list_destroy_node(l, l->head);
    _list_destroy_node(l, l->dummy);
    free(l);
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
    _list_destroy_node(l, node);
  }

  while (l->free != NULL) {
    node = l->free;
    l->free = node->next;
    lock_destroy(node->lock);
    free(node);
  }

  lock_destroy(l->freelock);
  free(l);
}


static void
_list_free_node(list_t *l, thread_t *thread, list_node_t *node)
{
  list_node_t **p = &l->free;

  lock(l->freelock, thread);

  if (l->op->free != NULL)
    l->op->free(node->item);
  node->next = l->free;
  *p = node;

  unlock(l->freelock, thread);
}


static void
_list_destroy_node(list_t *l, list_node_t *node)
{
  lock_destroy(node->lock);
  if (l->op->free != NULL)
    l->op->free(node->item);
  node->next = NULL;
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

