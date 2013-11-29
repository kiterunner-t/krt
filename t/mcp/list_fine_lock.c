/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include "list.c"


kerrno_t
list_insert(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t   *node;
  list_node_t   *cur;
  list_node_t   *prev;
  list_node_t   *head;
  list_node_t   *dummy;
  kitem_cmp_pt   cmp;
  long           n;

  dummy = l->dummy;
  head = l->head;
  cmp = l->op->cmp;

  prev = head;
  lock(prev->lock, thread);
  cur = prev->next;
  while (1) {
    lock(cur->lock, thread);
    if (cur == dummy) 
      break;

    n = cmp(item, cur->item);
    if (n == 0) {
      unlock(cur->lock, thread);
      unlock(prev->lock, thread);
      return KEEXIST;

    } else if (n > 0) {
      break;
    }

    unlock(prev->lock, thread);
    prev = cur;
    cur = cur->next;
  }

  node = _list_new_node(item, cur);
  if (node == NULL) {
    unlock(cur->lock, thread);
    unlock(prev->lock, thread);
    return KEMEM;
  }

  prev->next = node;
  unlock(cur->lock, thread);
  unlock(prev->lock, thread);
  return KSUCCESS;
}


kerrno_t
list_delete(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t   *cur;
  list_node_t   *prev;
  list_node_t   *head;
  list_node_t   *dummy;
  kitem_cmp_pt   cmp;
  long           n;
  kerrno_t       ret = KENOTFOUND;

  dummy = l->dummy;
  head = l->head;
  cmp = l->op->cmp;

  prev = head;
  lock(prev->lock, thread);
  cur = prev->next;
  while (1) {
    lock(cur->lock, thread);
    if (cur == dummy) 
      break;

    n = cmp(item, cur->item);
    if (n == 0) {
      prev->next = cur->next;
      cur->next = NULL;
      _list_free_node(l, thread, cur);
      ret = KSUCCESS;
      break;

    } else if (n > 0) {
      break;
    }

    unlock(prev->lock, thread);
    prev = cur;
    cur = cur->next;
  }

  unlock(cur->lock, thread);
  unlock(prev->lock, thread);
  return ret;
}


kitem_t
list_find(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t   *cur;
  list_node_t   *prev;
  list_node_t   *head;
  list_node_t   *dummy;
  kitem_t        item_found = KITEM_NULL;
  kitem_cmp_pt   cmp;
  long           n;

  dummy = l->dummy;
  head = l->head;
  cmp = l->op->cmp;

  prev = head;
  lock(prev->lock, thread);
  cur = prev->next;
  while (1) {
    lock(cur->lock, thread);
    if (cur == dummy) 
      break;

    n = cmp(item, cur->item);
    if (n == 0) {
      item_found = cur->item;
      break;
    } else if (n > 0) {
      break;
    }

    unlock(prev->lock, thread);
    prev = cur;
    cur = cur->next;
  }

  unlock(cur->lock, thread);
  unlock(prev->lock, thread);
  return item_found;
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

