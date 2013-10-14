/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmisc.h"
#include "item.h"
#include "item_long.h"
#include "item_string.h"
#include "splay_tree.h"


static void test_long(void);
static void test_string(void);


int
main(int argc, char **argv)
{
  printf("\n\n    test_long\n");
  test_long();

  printf("\n\n    test_string\n");
  test_string();
  return EXIT_SUCCESS;
}


static void
test_long(void)
{
  splay_tree_t *splay;
  //                    5  c   19  14  1e  f   18  d   12  10
  long          a[] = { 5, 12, 25, 20, 30, 15, 24, 13, 18, 16 };
  int           i;
  int           n;

  splay = splay_tree_init(&int_op);
  if (splay == NULL)
    kerror("init splay tree error");

  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    n = splay_tree_insert(splay, (void *) a[i]);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("splay tree insert error");
  }

  if (splay_tree_delete(splay, (void *) 30) != KSUCCESS)
    printf("splay tree delete error in 30\n");

  splay_tree_print(splay);
  splay_tree_destroy(splay);
}


static void
test_string(void)
{
  splay_tree_t *splay;
  string_t     *s;
  int           i;
  int           n;
  char         *sa[] = {
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

  splay = splay_tree_init(&string_op);
  if (splay == NULL)
    kerror("init splay tree error");

  for (i = 0; sa[i] != NULL; ++i) {
    s = string_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      kerror("malloc string_t error");

    n = splay_tree_insert(splay, (void *) s);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("splay tree insert [%s] error", sa[i]);
  }

  splay_tree_print(splay);
  splay_tree_destroy(splay);
}

