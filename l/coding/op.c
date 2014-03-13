#include <stdio.h>


int
int_shift_left2_rightn(int x, int n)
{
  x <<= 2;
  x >>= n;
  return x;
}


unsigned int
unsigned_shift_left2_rightn(unsigned int x, int n)
{
  x <<= 2;
  x >>= n;
  return x;
}


int
main(void)
{
  int      i = -1;         /* 0xFFFFFFFF */
  unsigned u = 0xFFFFFFFF;

  printf("int     : 0x%X\n", int_shift_left2_rightn(i, 2));
  printf("unsigned: 0x%X\n", unsigned_shift_left2_rightn(u, 2));
  return 0;
}


/*
OUTPUT:

int     : 0xFFFFFFFF
unsigned: 0x3FFFFFFF

从输出可知，有符号整数使用算术右移，而无符号数使用逻辑右移。对于左移效果一直，
都是使用逻辑左移。
*/

