/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"
#include "kitem_long.h"
#include "kitem_string.h"
#include "kd_tree.h"


static void test_kd(void);


int
main(int argc, char *argv[])
{
  printf("\n\n    test_kd\n");
  test_kd();
  return KSUCCESS;
}


static void
test_kd(void)
{
  int        i;
  kerrno_t   n;
  kd_tree_t *kd;
  kd_item_t  a[] = {
    { (kitem_t) 1,   (kitem_t) "hello" },
    { (kitem_t) 3,   (kitem_t) "world" },
    { (kitem_t) 9,   (kitem_t) "test" },
    { (kitem_t) 7,   (kitem_t) "hj" },
    { (kitem_t) 96,  (kitem_t) "krt" },
    { (kitem_t) 101, (kitem_t) "main" },
    { (kitem_t) 37,  (kitem_t) "master" }
  };

  kd = kd_tree_init(&g_long_op, &g_string_op);
  if (kd == NULL)
    kerror("init kd tree error");

  for (i = 0; i < sizeof(a)/sizeof(a[0]); ++i) {
    n = kd_tree_insert(kd, &a[i]);
    if (n != KSUCCESS)
      kerror("kd tree insert error");
  }

  kd_tree_destroy(kd);
}

