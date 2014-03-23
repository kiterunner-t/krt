/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <string.h>
#include "kmisc.h"
#include "kitem.h"
#include "binary_heap.h"


int
main(int argc, char **argv)
{
#include "kitem_string.h"

  int            i;
  binary_heap_t *bh;
  kstring_t     *s;
  char          *sa[] = {
    "hello",
    "world",
    "hj",
    "krt",
    "test",
    "10",
    "85",
    "15",
    "70",
    NULL
  };

  s = kstring_new(0, KSTRING_INVALID_LEN);
  if (s == NULL)
    kerror("string new error");

  bh = binary_heap_init(&g_string_op, 128);
  if (bh == NULL)
    kerror("init binary heap error");

  for (i = 0; sa[i] != NULL; ++i) {
    s = kstring_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      kerror("malloc string_t error");

    if (binary_heap_insert(bh, (kitem_t) s) != KSUCCESS)
      kerror("binary_heap_insert error");
  }

  binary_heap_print(bh);

  binary_heap_delete_min(bh);
  binary_heap_print(bh);

  binary_heap_destroy(bh);

  return 0;
}

