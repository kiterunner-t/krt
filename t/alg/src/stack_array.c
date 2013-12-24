/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"
#include "stack.h"


struct stack_s {
  size_t  top;
  size_t  capacity;
  kitem_t null;
  kitem_t items[1];
};


stack_t *
stack_init(size_t capacity, kitem_t null)
{
  stack_t *stack;

  if (capacity < 0)
    return NULL;

  stack = (stack_t *) malloc(sizeof(*stack) + sizeof(kitem_t) * capacity);
  if (stack == NULL)
    return NULL;

  stack->top = -1;
  stack->capacity = capacity;
  stack->null = null;
  return stack;
}


void
stack_destroy(stack_t *s)
{
  if (s != NULL)
    free(s);
}


kitem_t
stack_peek(stack_t *s)
{
  if (s->top < 0)
    return s->null;

  return s->items[s->top];
}


kitem_t
stack_pop(stack_t *s)
{
  if (s->top < 0)
    return s->null;

  return s->items[s->top--];
}


kerrno_t
stack_push(stack_t *s, kitem_t item)
{
  if (s->top == s->capacity)
    return KEEXCEED;

  s->items[++(s->top)] = item;
  return KSUCCESS;
}

