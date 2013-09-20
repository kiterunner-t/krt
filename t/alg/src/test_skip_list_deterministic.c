/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "misc.h"
#include "skip_list_deterministic.h"
#include "item.h"
#include "item_long.h"
#include "item_string.h"


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
  dskip_list_t *dsl;
  long          a[] = { 5, 10, 15, 20, 25, 27, 30, 35, 40, 45, 50 };
  int           i;
  void         *t;

  dsl = dskip_list_init(&int_op, (void *) LONG_MAX);
  if (dsl == NULL)
    error("init dskip list error");

  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    if (dskip_list_insert(dsl, (void *) a[i]) == -1)
      error("dskip list inert error");

    if (dskip_list_find(dsl, (void *) a[i], &t))
      error("skip find error");
  }

  if (dskip_list_insert(dsl, (void *) 5) != 1)
    error("dskip list insert duplicate key error");

  dskip_list_print(dsl);
  dskip_list_destroy(dsl);
}


static void
test_dskip_string()
{
  dskip_list_t  *dsl;
  string_t      *s;
  int            i;
  unsigned char *sa[] = {
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

  s = string_new(0, STRING_INVALID_LEN);
  if (s == NULL)
    error("string new error");

  dsl = dskip_list_init(&string_op, s);
  if (dsl == NULL)
    error("init dskip list error");

  for (i = 0; sa[i] != NULL; i++) {
    s = string_new(sa[i], strlen(sa[i]));
    if (s == NULL)
      error("malloc string_t error");

    if (dskip_list_insert(dsl, (void *) s) == -1)
      error("dskip_list insert error");
  }

  if (dskip_list_insert(dsl, (void *) s) == 1)
    printf("insert duplicate key\n");
  else
    error("error in insert duplicate key");

  dskip_list_print(dsl);
  dskip_list_destroy(dsl);
}

