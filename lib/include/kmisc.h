/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KMISC_H_
#define KMISC_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define KTRUE                                  1
#define KFALSE                                 0


#define KMAX(a, b)                            (((a) > (b)) ? (a) : (b))


#define KSUCCESS                               0
#define KEEXIST                                1
#define KENOTFOUND                             2
#define KEEMPTY                                3
#define KERROR                                -1
#define KEINVALID_PARAM                       -2
#define KEMEM                                 -3


#define KTERM_CLOSE                           "\033[0m"
#define KTERM_BOLD                            "\033[1m"
#define KTERM_TEXTCOLOR_BLACK                 "\033[30m"
#define KTERM_TEXTCOLOR_RED                   "\033[31m"
#define KTERM_TEXTCOLOR_GREEN                 "\033[32m"
#define KTERM_TEXTCOLOR_YELLOW                "\033[33m"
#define KTERM_TEXTCOLOR_BLUE                  "\033[34m"
#define KTERM_TEXTCOLOR_PURPLE                "\033[35m"
#define KTERM_TEXTCOLOR_CYAN                  "\033[36m"
#define KTERM_TEXTCOLOR_WHITE                 "\033[37m"
#define KTERM_BGCOLOR_BLACK                   "\033[40m"
#define KTERM_BGCOLOR_RED                     "\033[41m"
#define KTERM_BGCOLOR_GREEN                   "\033[42m"
#define KTERM_BGCOLOR_YELLOW                  "\033[43m"
#define KTERM_BGCOLOR_BLUE                    "\033[44m"
#define KTERM_BGCOLOR_PURPLE                  "\033[45m"
#define KTERM_BGCOLOR_CYAN                    "\033[46m"
#define KTERM_BGCOLOR_WHITE                   "\033[47m"

#define KERROR_FLAG                                                 \
    KTERM_BOLD KTERM_BGCOLOR_BLACK KTERM_TEXTCOLOR_RED              \
    "[ERROR] " KTERM_CLOSE


#define kerror(fmt, ...)                                            \
  do {                                                              \
    fprintf(stderr, "[ERROR] in <%s/%s/%d>: " fmt "\n",             \
            __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);       \
    abort();                                                        \
  } while (0)


typedef int kbool_t;
typedef int kerrno_t;


#endif

