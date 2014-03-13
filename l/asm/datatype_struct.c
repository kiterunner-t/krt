#include <stdio.h>


#define offsetof(type, field) ((size_t) &(((type *) 0)->field))


typedef struct point_s point_t;

struct point_s {
  int x;
  int y;
};


int
main(void)
{
  int a = offsetof(point_t, y);
  printf("%d\n", a);
  return 0;
}

