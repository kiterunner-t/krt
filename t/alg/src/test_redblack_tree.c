/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "misc.h"
#include "redblack_tree.h"
#include "item_long.h"
#include "item_string.h"


static void test_long();
static void test_string();


int
main(int argc, char **argv)
{
  printf("test long redblack: \n");
  test_long();

  printf("\n");
  printf("test string redblack: \n");
  test_string();
  return 0;
}


static void
test_long()
{
  redblack_tree_t *rb;
  long             a[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };
  int              i;

  rb = redblack_tree_init(&int_op, (void *) LONG_MAX);
  if (rb == NULL) 
    error("init redblack tree error");

  for (i = 0; i < sizeof(a) / sizeof(long); ++i) {
    if (redblack_tree_insert(rb, (void *) a[i]) != 0)
      error("redblack tree insert error");
  }

  redblack_tree_print(rb);
  printf("the size of the redblack tree is %lu, rotate count is %lu\n", 
         redblack_tree_size(rb), redblack_tree_rotate_count(rb));

  printf("delete nonexist item: 100\n");
  redblack_tree_delete(rb, (void *) 100);
  redblack_tree_print(rb);

  for (i = sizeof(a) / sizeof(long) - 1; i >= 0; --i) {
    redblack_tree_delete(rb, (void *) a[i]);
    printf("\n\ndelete %ld--------------\n", a[i]);
    redblack_tree_print(rb);
  }

  redblack_tree_destroy(rb);
}


static void
test_string()
{
  redblack_tree_t *rb;
  string_t        *s;
  int              i;
  char            *sa[] = {
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

  s = string_new(0, STRING_INVALID_LEN);
  if (s == NULL)
    error("string new error");

  rb = redblack_tree_init(&string_op, s);
  if (rb == NULL) 
    error("init redblack tree error");

  for (i = 0; sa[i] != NULL; i++) {
    s = string_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      error("malloc string_t error");

    if (redblack_tree_insert(rb, (void *) s) != 0)
      error("redblack insert error");
  }

  printf("the size of the redblack tree is %lu, rotate count is %lu\n", 
         redblack_tree_size(rb), redblack_tree_rotate_count(rb));

  redblack_tree_delete(rb, (void *) s);
  redblack_tree_print(rb);
  redblack_tree_destroy(rb);
}

