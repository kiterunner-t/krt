#include <stdio.h>
#include <stdlib.h>
#include <string.h>


float
sum(float a[], unsigned len)
{
  int i;
  float result = 0.0;

// if len = 0, dump
//  for (i = 0; i <= len-1; ++i)
  for (i = 0; i < len; ++i)
    result += a[i];

  return result;
}


float
sum2(float a[], int len)
{
  int i;
  float result = 0.0;

  for (i = 0; i < len-1; ++i)
    result += a[i];

  return result;
}


int
strlonger_error(char *s, char *t)
{
  return strlen(s) - strlen(t) > 0;
}


int
strlonger(char *s, char *t)
{
  return strlen(s) > strlen(t);
}


#define KSIZE 1024

// be care of size_t
// void *memecpy(void *dest, void *src, size_t n);
int
freebsd_getpeername_bug(void *user_dest, int maxlen)
{
  int  len = KSIZE<maxlen ? KSIZE : maxlen;
  char kbuf[KSIZE];

  memcpy(user_dest, kbuf, len);
  return len;
}


// XDR bug
void *
copy_elements_bug(void *ele_src[], int ele_cnt, size_t ele_size)
{
  void *result;
  void *next;
  int   i;

  /*
  long long unsigned required_size = ele_cnt * (long long unsigned) ele_size;
  size_t request_size = (size_t) required_size;
  if (required_size != request_size)
    return NULL;

  result = malloc(request_size);
  */

  /*
  long long unsiged asize = ele_cnt * (long long unsigned) ele_size;
  result = malloc(asize);
  */

  result = malloc(ele_cnt * ele_size);
  if (result == NULL)
    return NULL;

  next = result;
  for (i = 0; i < ele_cnt; i++) {
    memcpy(next, ele_src[i], ele_size);
    next += ele_size;
  }

  return result;
}


long
gfun(int x, int y)
{
  long t1 = (long) x + y;
  long t2 = (long) (x + y);
  return t1 | t2;
}


int
main(void)
{
  float b = 1;
  float result;

  result = sum(&b, 0);
  result = sum2(&b, 0);

  char *s = "1";
  char *t = "22";
  printf("%d\n", strlonger_error(s, t));
  printf("%d\n", strlonger(s, t));
  return 0;
}

