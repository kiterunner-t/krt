#include "kvarg.h"


/*
static void
_varg(int num, ...)
{
  kva_list_t ap;
  char       c;

  ap = (kva_list_t) &num + KTYPE_ALIGN_INT(num); // 获取第一个可变参数的地址
  ap += KTYPE_ALIGN_INT(char);                   // ap 指向下一个参数的地址
  c = *(char *) (ap - KTYPE_ALIGN_INT(char));    // 获取当前参数的值
  ap = NULL;
}
*/

static void
_varg(int num, ...)
{
  kva_list_t  ap;
  char        c1;
  char        c2;

  kva_start(ap, num);
  c1 = kva_arg(ap, char);
  c2 = kva_arg(ap, char);
  kva_end(ap);
}


static void
_t_varg(void)
{
  int   i = 2;
  short s = 1;
  char  c1 = 'a';
  char  c2 = 'b';

  _varg(5, c1, c2, s, i, &i);
}

