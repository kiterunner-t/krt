#include <stdio.h>
#include <string.h>


#define KENDIAN_BIG     0
#define KENDIAN_LITTLE  1
#define KENDIAN_ERROR   -1


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


typedef unsigned char *byte_pointer_t;


void
show_bytes(byte_pointer_t start, int len)
{
  int i;
  for (i = 0; i < len; ++i)
    printf(" %.2x", start[i]);
  printf("\n");
}


void
show_int(int x)
{
  show_bytes((byte_pointer_t) &x, sizeof(int));
}


void
show_float(float x)
{
  show_bytes((byte_pointer_t) &x, sizeof(float));
}


void
show_pointer(void *x)
{
  show_bytes((byte_pointer_t) &x, sizeof(void *));
}


void
test_show_bytes(int val)
{
  int    ival = val;
  float  fval = (float) ival;
  int   *pval = &ival;

  show_int(ival);
  show_float(fval);
  show_pointer(pval);

  const char s[] = "abcdef";
  show_bytes((byte_pointer_t) s, sizeof(s));
  show_bytes((byte_pointer_t) s, strlen(s));
}


/* bit expand: sign or zero expand */
void
test_show_bit_expand(void)
{
  short          sx = -12345;
  unsigned short usx = sx;
  int            x = sx;
  unsigned       ux = usx;

  printf("sx  = %d:\t", sx);
  show_bytes((byte_pointer_t) &sx, sizeof(sx));

  printf("usx = %u:\t", usx);
  show_bytes((byte_pointer_t) &usx, sizeof(usx));

  printf("x   = %d:\t", x);
  show_bytes((byte_pointer_t) &x, sizeof(x));

  printf("ux  = %u:\t", ux);
  show_bytes((byte_pointer_t) &ux, sizeof(ux));
}


void
test_show_mystery_expand(void)
{
  short    sx = -12344;
  unsigned uy = (unsigned) ((unsigned short) sx);
  unsigned uy2 = (unsigned) ((int) sx);
  unsigned uy3 = sx;

  printf("uy  = %u:\t", uy);
  show_bytes((byte_pointer_t) &uy, sizeof(uy));

  printf("uy2 = %u:\t", uy2);
  show_bytes((byte_pointer_t) &uy2, sizeof(uy2));

  printf("uy3 = %u:\t", uy3);
  show_bytes((byte_pointer_t) &uy3, sizeof(uy3));
}


int
main(void)
{
  int endian;

  endian = get_host_endian();
  if (endian == KENDIAN_BIG)
    printf("the host is big endian\n");
  else if (endian == KENDIAN_LITTLE)
    printf("the host is little endian\n");
  else
    printf("error get_host_endian\n");

  printf("\n");
  test_show_bytes(12345);

  printf("\ntest_show_bit_expand\n");
  test_show_bit_expand();

  printf("\ntest_show_mystery_expand\n");
  test_show_mystery_expand();
  return 0;
}

