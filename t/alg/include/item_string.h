/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef ITEM_STRING_H_
#define ITEM_STRING_H_

#include <stdlib.h>
#include "item.h"

#define STRING_INVALID_LEN  -1


typedef struct string_s string_t;

extern item_op_t string_op;


string_t *string_new(unsigned char *s, size_t len);


#endif

