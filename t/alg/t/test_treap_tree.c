/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmisc.h"
#include "kitem.h"
#include "treap_tree.h"


static void test_long_treap(void);
static void test_string_treap(void);


int
main(int argc, char **argv)
{
  printf("\n\n    test_long_treap\n");
  test_long_treap();

  printf("\n\n    test_string_treap\n");
  test_string_treap();
  return EXIT_SUCCESS;
}


static void
test_long_treap(void)
{
#include "kitem_long.h"

  treap_tree_t *treap;
  //                    5  c   19  14  1e  f   18  d   12  10
  long          a[] = { 5, 12, 25, 20, 30, 15, 24, 13, 18, 16 };
  int           i;
  int           n;

  treap = treap_tree_init(&g_long_op);
  if (treap == NULL)
    kerror("init treap tree error");

  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    n = treap_tree_insert(treap, (kitem_t) a[i]);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("treap tree insert error");
  }

  if (treap_tree_delete(treap, (kitem_t) 119) == KENOTFOUND)
    printf("delete 119, not found\n");
  else
    kerror("treap tree delete error in 119\n");

  treap_tree_print(treap);
  printf("begin delete -------------------------\n");
  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    if (treap_tree_delete(treap, (kitem_t) a[i]) != KSUCCESS)
      kerror("treap tree delete item [%ld] error", a[i]);
  }

  treap_tree_print(treap);
  treap_tree_destroy(treap);
}


static void
test_string_treap()
{
#include "kitem_string.h"

  treap_tree_t *treap;
  kstring_t    *s;
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

  treap = treap_tree_init(&g_string_op);
  if (treap == NULL)
    kerror("init treap tree error");

  for (i = 0; sa[i] != NULL; ++i) {
    s = kstring_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      kerror("malloc string_t error");

    n = treap_tree_insert(treap, (void *) s);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("treap tree insert [%s] error", sa[i]);
  }

  if (treap_tree_delete(treap, s) != KSUCCESS)
    kerror("treap tree delete error");

  treap_tree_print(treap);
  treap_tree_destroy(treap);
}

