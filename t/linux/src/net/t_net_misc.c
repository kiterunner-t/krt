#include <stdio.h>
#include "net_misc.h"


int
main()
{
  int endian = get_host_endian();

  if (endian == KENDIAN_LITTLE)
    printf("little endian\n");
  else if (endian == KENDIAN_BIG)
    printf("big endian\n");
  else
    printf("error\n");

  return 0;
}

