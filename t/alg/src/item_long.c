/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include "item.h"
#include "item_long.h"


static inline long int_cmp(void *item1, void *item2);
static inline void int_print(void *item);


item_op_t int_op = {
  int_cmp,
  int_print,
  NULL
};


static inline long
int_cmp(void *item1, void *item2)
{
  return (long) item1 - (long) item2;
}


static inline void
int_print(void *item)
{
  printf("%ld", (long) item);
}

