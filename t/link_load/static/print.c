/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include "c.h"


void
print_(const char *s, unsigned long len)
{
#ifdef x86_64
  __asm__ __volatile__(
    "mov %0, %%rdx;"
    "mov %1, %%rsi;"
    "mov $0x1, %%rdi;"
    "mov $0x1, %%rax;"
    "syscall;"
    :
    : "r" (len), "r" (s)
    : "rdx", "rsi", "rdi"
  );

#elif defined x86_32
  __asm__ __volatile__(
    "movl %0, %%edx;"
    "movl %1, %%ecx;"
    "movl $0x0, %%ebx;"
    "movl $0x4, %%eax;"
    "int $0x80;"
    :
    : "r" (len), "r" (s)
    : "edx", "ecx", "ebx"
  );

#else
  #include <stdio.h>
  printf(s);
#endif
}

