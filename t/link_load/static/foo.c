/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include "c.h"

short shared;
int   a = 1;


int
foo(void)
{
  char str[5];

  swap(&a, &shared);

  str[0] = a + 0x30;
  str[1] = ',';
  str[2] = shared + 0x30;
  str[3] = '\n';
  str[4] = 0;
  print_(str, 5);
  exit_();
}

