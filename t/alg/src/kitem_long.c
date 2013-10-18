/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include "kitem.h"
#include "kitem_long.h"


static inline long long_cmp(kitem_t item1, kitem_t item2);
static inline long long_cmp_key1(kitem_t item, kkey_t key);
static inline void long_print(kitem_t item);


kitem_op_t g_long_op = {
  long_cmp,
  long_cmp_key1,
  long_print,
  NULL
};


static inline long
long_cmp(kitem_t item1, kitem_t item2)
{
  return (long) item1 - (long) item2;
}


static inline long
long_cmp_key1(kitem_t item, kkey_t key)
{
  return (long) item - (long) key;
}


static inline void
long_print(kitem_t item)
{
  printf("%ld", (long) item);
}

