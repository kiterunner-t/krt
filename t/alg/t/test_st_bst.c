/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmisc.h"
#include "kitem.h"
#include "st.h"


static void test_long_st(void);
static void test_string_st(void);


int
main(int argc, char **argv)
{
//#define RECURSION
  printf("\n\n    test_long_st\n");
  test_long_st();

  printf("\n\n    test_string_st\n");
  test_string_st();
  return EXIT_SUCCESS;
}


static void
test_long_st(void)
{
#include "kitem_long.h"

  //                  5  c   19  14  1e  f   18  d   12  10
  long        a[] = { 5, 12, 25, 20, 30, 15, 24, 13, 18, 16, 9, 10, 32, 72, 118, 123, 456, 77, 56, 34, 43 };
  int         i;
  int         n;
  kerrno_t    ret;

  ret = st_init(&g_long_op);
  if (ret != KSUCCESS)
    kerror("init st error: %d", ret);

  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    n = st_insert((kitem_t) a[i]);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("st insert error");
  }

  st_delete((kitem_t) 119);


  st_sort(g_long_op.print);
  printf("begin delete -------------------------\n");
  for (i = 0; i < sizeof(a)/sizeof(long); ++i) {
    st_delete((kitem_t) a[i]);
    printf("after delete [%ld/%ld]\n", a[i], st_size());
    st_sort(g_long_op.print);
    printf("------------------------\n\n");
  }

  st_sort(g_long_op.print);
  st_destroy();
}


static void
test_string_st()
{
#include "kitem_string.h"

  kstring_t  *s;
  int         i;
  int         n;
  kerrno_t    ret;
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

  ret = st_init(&g_string_op);
  if (ret != KSUCCESS)
    kerror("init st error: %d", ret);

  for (i = 0; sa[i] != NULL; ++i) {
    s = kstring_new((unsigned char *) sa[i], strlen(sa[i]));
    if (s == NULL)
      kerror("malloc string_t error");

    n = st_insert((void *) s);
    if (n == KEEXIST)
      printf("[NOTICE] insert duplicate key\n");
    else if (n != KSUCCESS)
      kerror("st insert [%s] error", sa[i]);
  }

  st_delete(s);

  st_sort(g_string_op.print);
  st_destroy();
}

