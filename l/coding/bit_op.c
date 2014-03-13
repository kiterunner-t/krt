#include <stdio.h>


int
main(void)
{
  int w = 0xFFFFFFFF;
  int k = 32;

  printf("    w: %x\n", w);
  printf("w<<32: %x\n", w << k);
  printf("w<< 1: %x\n", w << 1);
  printf("w<<33: %x\n", w << (k+1));
  return 0;
}


/*
OUTPUT:

    w: ffffffff
w<<32: ffffffff
w<< 1: fffffffe
w<<33: fffffffe
*/

