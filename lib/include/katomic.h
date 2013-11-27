/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KATOMIC_H_
#define KATOMIC_H_


#if (KUSE_GCC_ATOMIC)
# include "katomic_gcc.h"
#else
# error "unknown atomic"
#endif


// #define kbarrier() __asm__ __volatile__("" : : : "memory")


#endif

