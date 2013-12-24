/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <string.h>
#include "kitem.h"
#include "kitem_string.h"


static inline long string_cmp(kitem_t item1, kitem_t item2);
static inline long string_cmp_key1(kitem_t item, kkey_t key);
static inline void string_print(kitem_t item);
static inline void string_free(kitem_t item);


kitem_op_t g_string_op = {
  KITEM_STRING_NULL,

  string_cmp,
  string_cmp_key1,
  string_print,
  string_free
};


struct kstring_s {
  unsigned char *str;
  long           len;
};


kstring_t *
kstring_new(unsigned char *s, size_t len)
{
  kstring_t *str;

  str = (kstring_t *) malloc(sizeof(*str));
  if (str == NULL)
    return NULL;

  str->str = s;
  str->len = len;
  return str;
}


static inline long
string_cmp(kitem_t item1, kitem_t item2)
{
  kstring_t *s1 = (kstring_t *) item1;
  kstring_t *s2 = (kstring_t *) item2;
  size_t     n;

  if (s1->len==KSTRING_INVALID_LEN && s2->len==KSTRING_INVALID_LEN)
    return 0;
  if (s1->len == KSTRING_INVALID_LEN)
    return 1;
  if (s2->len == KSTRING_INVALID_LEN)
    return -1;

  n = (s1->len > s2->len) ? s2->len : s1->len;
  return (long) strncmp((char *) s1->str, (char *) s2->str, n);
}


static inline long
string_cmp_key1(kitem_t item, kkey_t key)
{
  kstring_t *s = (kstring_t *) item;
  char      *k = (char *) key;

  if (s->len == KSTRING_INVALID_LEN)
    return 1;
  if (k == NULL)
    return -1;

  return (long) strcmp((char *) s->str, k);
}


static inline void
string_print(kitem_t item)
{
  kstring_t *s = (kstring_t *) item;
  printf("%s", (s->len > 0) ? ((char *) (s->str)) : "<0>");
}


static inline void
string_free(kitem_t item)
{
  if (item)
    free(item);
}

