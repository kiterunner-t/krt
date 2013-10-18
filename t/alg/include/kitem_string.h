/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KITEM_STRING_H_
#define KITEM_STRING_H_

#include <stdlib.h>
#include "kitem.h"

#undef KITEM_NULL
#define KITEM_NULL            NULL

#define KSTRING_INVALID_LEN  -1


typedef struct kstring_s kstring_t;

extern kitem_op_t g_string_op;


kstring_t *kstring_new(unsigned char *s, size_t len);


#endif

