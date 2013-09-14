/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef DLIST_H_
#define DLIST_H_

#include "misc.h"


typedef struct dlist_s dlist_t;

struct dlist_s {
  dlist_t *prev;
  dlist_t *next;
};


#define DLIST_INIT(dl) { dl, dl }

#define DLIST_FOREACH(head, node)       \
    for (node = (head)->next; node != (head); node = node->next)

/*
#define DLIST_ENTRY(node, type, member) 
  (type *) ((node) - offsetof(type, member))
*/

#define DLIST_ENTRY(node, type, member) \
  (type *) ((node) - (((type) *) 0)->(member))


static inline void
_dlist_del(dlist_t *prev, dlist_t *next)
{
  next->prev = prev;
  prev->next = next;
}


static inline void
_dlist_add(dlist_t *prev, dlist_t *next, dlist_t *entry)
{
  entry->prev = prev;
  entry->next = next;
  prev->next = entry;
  next->prev = entry;
}


static inline void
dlist_init(dlist_t *dl)
{
  dl->prev = dl;
  dl->next = dl;
}


static inline bool_t
dlist_isempty(dlist_t *head)
{
  return (bool_t) (head == head->next);
}


static inline void
dlist_del(dlist_t *node)
{
  _dlist_del(node->prev, node->next);
}


static inline void
dlist_add(dlist_t *head, dlist_t *entry)
{
  _dlist_add(head, head->next, entry);
}


static inline void
dlist_add_tail(dlist_t *head, dlist_t *entry)
{
  _dlist_add(head, head->prev, entry);
}


#endif

