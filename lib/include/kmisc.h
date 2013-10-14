/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KMISC_H_
#define KMISC_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define KTRUE  1
#define KFALSE 0


#define KMAX(a, b) (((a) > (b)) ? (a) : (b))


#define KSUCCESS               0
#define KEEXIST                1
#define KENOTFOUND             2
#define KEEMPTY                3
#define KERROR                -1
#define KEINVALID_PARAM       -2
#define KEMEM                 -3


#define kerror(fmt, ...)                                            \
  do {                                                              \
    fprintf(stderr, "[ERROR] in <%s/%s/%d>: " fmt "\n",             \
            __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);       \
    abort();                                                        \
  } while (0)


typedef int bool_t;
typedef int errno_t;


#endif

