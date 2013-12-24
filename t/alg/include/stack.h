/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"


typedef struct stack_s stack_t;


extern stack_t  *stack_init(size_t capacity, kitem_t null);
extern void      stack_destroy(stack_t *s);
extern kitem_t   stack_peek(stack_t *s);
extern kitem_t   stack_pop(stack_t *s);
extern kerrno_t  stack_push(stack_t *s, kitem_t item);


#endif

