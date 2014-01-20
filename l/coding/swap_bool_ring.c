#include <stdio.h>


void
swap_bool_ring(int *x, int *y)
{
  *y = *x ^ *y;
  *x = *x ^ *y;
  *y = *x ^ *y;
}


int
main(void)
{
  int x = 1;
  int y = 2;
  int z = 3;

  printf("x = %d, y = %d\n", x, y);
  swap_bool_ring(&x, &y);
  printf("x = %d, y = %d\n", x, y);

  printf("\n");
  printf("z = %d\n", z);
  swap_bool_ring(&z, &z);
  printf("z = %d\n", z);
  return 0;
}


