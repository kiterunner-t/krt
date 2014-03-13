#ifndef KVARG_H_
#define KVARG_H_

#include <stdlib.h>

typedef unsigned char *kva_list_t;


#define KTYPE_ALIGN(v, n)   ((sizeof(v) + sizeof(n) - 1) & ~(sizeof(n) - 1))
#define KTYPE_ALIGN_INT(v)  KTYPE_ALIGN(v, sizeof(int))

#define kva_start(ap, v)    (ap = (kva_list_t) &v + KTYPE_ALIGN_INT(v))
#define kva_arg(ap, type)   \
    (*(type *) ((ap += KTYPE_ALIGN_INT(type)) - KTYPE_ALIGN_INT(type)))

#define kva_end(ap)         (ap = NULL)


#endif

