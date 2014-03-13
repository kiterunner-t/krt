#include <stdio.h>

struct st {
  short w[3];
  char c[3];
};

struct s2 {
  short i;
  char  c;
  int   j;
};


int
slen()
{
  struct st s1[2];
  struct st *pst, *pst1;
  int sa = sizeof(s1);
  pst = &s1[0];
  pst1 = &s1[1];
  printf("pst: %x, pst1: %x\n", pst, pst1);
  printf("sizeof(s2): %d\n", sizeof(struct s2));
  return sizeof(struct st);
}


int
main()
{
  slen();
  return 0;
}

