/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmisc.h"
#include "kitem.h"
#include "avl_tree.h"


static void test_long_avl(void);
static void test_string_avl(void);


int
main(int argc, char **argv)
{
  printf("\n\n    test_long_avl\n");
  test_long_avl();

  printf("\n\n    test_string_avl\n");
  test_string_avl();
  return EXIT_SUCCESS;
}


static void
test_long_avl(void)
{
#include "kitem_long.h"

  avl_tree_t *avl;
  //                  5  c   19  14  1e  f   18  d   12  10
  long        a[] = { 5, 12, 25, 20, 30, 15, 24, 13, 18, 16, 9, 10, 32, 72, 118, 123, 456, 77, 56, 34, 43 };
  int         i;
  int         n;

  avl = avl_tree_init(&g_long_op);
  if (avl == NULL)
    kerror("init avl tree error");

  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    n = avl_tree_insert(avl, (kitem_t) a[i]);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("avl tree insert error");
  }

  if (avl_tree_delete(avl, (kitem_t) 119) == KENOTFOUND)
    printf("delete 119, not found\n");
  else
    printf(KERROR_FLAG "avl tree delete error in 119\n");

  avl_tree_print(avl);
  printf("begin delete -------------------------\n");
  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    if (avl_tree_delete(avl, (kitem_t) a[i]) != KSUCCESS)
      printf(KERROR_FLAG " avl tree delete error in %ld\n", a[i]);
    printf("after delete [%ld]\n", a[i]);
    avl_tree_print(avl);
    printf("------------------------\n\n");
  }

  avl_tree_print(avl);
  avl_tree_destroy(avl);
}


static void
test_string_avl()
{
#include "kitem_string.h"

  avl_tree_t *avl;
  kstring_t  *s;
  int         i;
  int         n;
  char       *sa[] = {
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

  avl = avl_tree_init(&g_string_op);
  if (avl == NULL)
    kerror("init avl tree error");

  for (i = 0; sa[i] != NULL; ++i) {
    s = kstring_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      kerror("malloc string_t error");

    n = avl_tree_insert(avl, (void *) s);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("avl tree insert [%s] error", sa[i]);
  }

  if (avl_tree_delete(avl, s) != KSUCCESS)
    kerror("avl tree delete error");

  avl_tree_print(avl);
  avl_tree_destroy(avl);
}

