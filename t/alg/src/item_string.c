/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <string.h>
#include "item.h"
#include "item_string.h"


static inline long string_cmp(void *item1, void *item2);
static inline void string_print(void *item);
static inline void string_free(void *item);


item_op_t string_op = {
  string_cmp,
  string_print,
  string_free
};


struct string_s {
  unsigned char *str;
  long           len;
};


string_t *
string_new(unsigned char *s, size_t len)
{
  string_t *str;

  str = (string_t *) malloc(sizeof(*str));
  if (str == NULL)
    return NULL;

  str->str = s;
  str->len = len;
  return str;
}


static inline long
string_cmp(void *item1, void *item2)
{
  string_t *s1 = (string_t *) item1;
  string_t *s2 = (string_t *) item2;
  size_t    n;

  if (s1->len==STRING_INVALID_LEN && s2->len==STRING_INVALID_LEN)
    return 0;
  if (s1->len == STRING_INVALID_LEN)
    return 1;
  if (s2->len == STRING_INVALID_LEN)
    return -1;

  n = (s1->len > s2->len) ? s2->len : s1->len;
  return (long) strncmp(s1->str, s2->str, n);
}


static inline void
string_print(void *item)
{
  string_t      *s = (string_t *) item;
  unsigned char *t = "<0>";

  printf("%s", (s->len > 0) ? s->str : t);
}


static inline void
string_free(void *item)
{
  if (item)
    free(item);
}

