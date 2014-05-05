/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include "list.c"


static kbool_t _list_validate(list_t *l, list_node_t *prev, list_node_t *cur);


kerrno_t
list_insert(list_t *l, thread_t *thread, kitem_t item)
{
  list_node_t  *dummy = l->dummy;
  list_node_t  *prev;
  list_node_t  *cur;
  list_node_t  *node;
  kerrno_t      ret = KSUCCESS;
  long          n = -1;
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
  long          n = -1;
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
        _list_free_node(l, thread, cur);
        unlock(cur->lock, thread);
        unlock(prev->lock, thread);
        return KSUCCESS;
      }

      unlock(cur->lock, thread);
      unlock(prev->lock, thread);
      return KENOTFOUND;
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
  kitem_t       ret = l->op->null;
  long          n = -1;
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

