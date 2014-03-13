#include <stdio.h>


char b[] = "hello, world!";
char *a = "kiterunner_t";


int
main(void)
{
  printf("%s\n", a);
  b[1] = 'E';
//  a[1] = 'E';
  printf("%s\n", a);
  return 0;
}

