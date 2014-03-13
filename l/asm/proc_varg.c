#include <stdio.h>
#include "kvarg.h"


void
varg(int n, ...)
{
  int         i;
  kva_list_t  ap;
  char        c1;
  char        c2;
  short       vs;
  int        *pi;

  kva_start(ap, n);
  for (i = 0; i < n; i++) {
    switch (i) {
    case 0:
      c1 = kva_arg(ap, char);
      printf("character: %c\n", c1);
      break;

    case 1:
      c2 = kva_arg(ap, char);
      printf("character: %c\n", c2);
      break;

    case 2:
      vs = kva_arg(ap, short);
      printf("    short: %d\n", vs);
      break;

    case 3:
      pi = kva_arg(ap, int *);
      printf("    int *: 0x%X(%d)\n", pi, *pi);
      break;

    default:
      break;
    }
  }
  kva_end(ap);
  printf("\n");
}


int
main(void)
{
  short s = 3;
  char  c1 = 'a';
  char  c2 = 'b';
  int   i = 4;

  varg(0);
  varg(1, c1);
  varg(2, c1, c2);
  varg(3, c1, c2, s);
  varg(4, c1, c2, s, &i);
  return 0;
}

