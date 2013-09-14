/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef MISC_H_
#define MISC_H_


#include <stdio.h>
#include <stdlib.h>


#define TRUE  1
#define FALSE 0



#define error(msg) \
  do { \
    fprintf(stderr, "error in %d line of [%s]: %s\n", __LINE__, __FILE__, msg); \
    abort(); \
  } while (0)


typedef int bool_t;


#endif

