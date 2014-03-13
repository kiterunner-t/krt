#include <stdio.h>

int
main()
{
  char         a = 254;
  unsigned int b = (unsigned int) a;
  
  printf("%x -> %x\n", (char) a, b);
  return 0;
}

