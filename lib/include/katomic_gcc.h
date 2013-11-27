/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KATOMIC_GCC_H_
#define KATOMIC_GCC_H_


typedef long                    katomic_int_t;
typedef unsigned long           katomic_uint_t;
typedef volatile katomic_uint_t katomic_t;

#define katomic_cmp_set(lock, old, new)                        \
    __sync_val_compare_and_swap(lock, old, new)

#define katomic_fetch_add(value, add)  __sync_fetch_and_add(value, add)
#define kbarrier()                     __sync_synchronize()


#endif

