#include "net_misc.h"


int
get_host_endian()
{
  union {
    short a;
    char  b[sizeof(short)];
  } order;

  order.a = 0x0102;
  if (order.b[0]==0x01 && order.b[1]==0x02)
    return KENDIAN_BIG;
  else if (order.b[1]==0x01 && order.b[0]==0x02)
    return KENDIAN_LITTLE;
  else
    return KENDIAN_ERROR;
}

