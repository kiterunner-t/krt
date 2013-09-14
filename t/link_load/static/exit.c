/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include "c.h"


void
exit_(void)
{
#ifdef x86_64
  __asm__ __volatile__(
    "mov $0x2A, %rdi;"
    "mov $0x3C, %rax;"
    "syscall;"
  );

#elif defined x86_32
  __asm__ __volatile__(
    "movl $0x2A, %ebx;"
    "movl $0x1, %eax;"
    "int $0x80;"
  );

#else
  #include <stdlib.h>
  exit(0x2A);
#endif
}

