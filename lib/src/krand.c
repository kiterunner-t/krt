/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include <time.h>
#include "krand.h"


void
krand_init(void)
{
  srand(time(NULL));
}


long
krand_get(void)
{
  return (long) rand(); // for now
}

