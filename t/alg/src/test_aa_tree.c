/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmisc.h"
#include "kitem.h"
#include "kitem_long.h"
#include "kitem_string.h"
#include "aa_tree.h"


static void test_long_aa(void);
static void test_string_aa(void);


int
main(int argc, char **argv)
{
  printf("\n\n    test_long_aa\n");
  test_long_aa();

  printf("\n\n    test_string_aa\n");
  test_string_aa();
  return EXIT_SUCCESS;
}


static void
test_long_aa(void)
{
  aa_tree_t *aa;
    //                    5  c   19  14  1e  f   18  d   12  10
  long          a[] = { 5, 12, 25, 20, 30, 15, 24, 13, 18, 16 };
  int           i;
  int           n;

  aa = aa_tree_init(&g_long_op);
  if (aa == NULL)
    kerror("init aa tree error");

  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    n = aa_tree_insert(aa, (void *) a[i]);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("aa tree insert error");
  }

  if (aa_tree_delete(aa, (void *) 30) != KSUCCESS)
    printf("aa tree delete error in 30\n");

  aa_tree_print(aa);
  aa_tree_destroy(aa);
}


static void
test_string_aa()
{
  aa_tree_t  *aa;
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

  aa = aa_tree_init(&g_string_op);
  if (aa == NULL)
    kerror("init aa tree error");

  for (i = 0; sa[i] != NULL; ++i) {
    s = kstring_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      kerror("malloc string_t error");

    n = aa_tree_insert(aa, (void *) s);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("aa tree insert [%s] error", sa[i]);
  }

  if (aa_tree_delete(aa, s) != KSUCCESS)
    kerror("aa tree delete error");
  aa_tree_print(aa);
  aa_tree_destroy(aa);
}

