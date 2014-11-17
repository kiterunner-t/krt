/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "kmisc.h"
#include "skip_list_deterministic.h"
#include "kitem.h"


static void test_dskip_long();
static void test_dskip_string();


int
main(int argc, char **argv)
{
  test_dskip_long();
  test_dskip_string();
  return EXIT_SUCCESS;
}


static void
test_dskip_long()
{
#include "kitem_long.h"

  dskip_list_t *dsl;
  long          a[] = { 5, 10, 15, 20, 25, 27, 30, 35, 40, 45, 50 };
  int           i;
  kitem_t       t;

  dsl = dskip_list_init(&g_long_op, (kitem_t) LONG_MAX);
  if (dsl == NULL)
    kerror("init dskip list error");

  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    if (dskip_list_insert(dsl, (void *) a[i]) == -1)
      kerror("dskip list inert error");

    if (dskip_list_find(dsl, (void *) a[i], &t))
      kerror("skip find error");
  }

  if (dskip_list_insert(dsl, (void *) 5) != 1)
    kerror("dskip list insert duplicate key error");

  dskip_list_print(dsl);
  dskip_list_destroy(dsl);
}


static void
test_dskip_string()
{
#include "kitem_string.h"

  dskip_list_t  *dsl;
  kstring_t     *s;
  int            i;
  char          *sa[] = {
    "hello",
    "world",
    "hj",
    "krt",
    "test",
    "10",
    "85",
    "75",
    NULL
  };

  s = kstring_new(0, KSTRING_INVALID_LEN);
  if (s == NULL)
    kerror("string new error");

  dsl = dskip_list_init(&g_string_op, s);
  if (dsl == NULL)
    kerror("init dskip list error");

  for (i = 0; sa[i] != NULL; i++) {
    s = kstring_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      kerror("malloc string_t error");

    if (dskip_list_insert(dsl, (void *) s) == -1)
      kerror("dskip_list insert error");
  }

  if (dskip_list_insert(dsl, (void *) s) == 1)
    printf("insert duplicate key\n");
  else
    kerror("error in insert duplicate key");

  dskip_list_print(dsl);
  dskip_list_destroy(dsl);
}

