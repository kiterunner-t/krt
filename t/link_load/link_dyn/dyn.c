#include <stdio.h>

extern void call_foo();
extern int g_extern;


int
main(void)
{
  g_extern = 42; /* 7.3.4 共享模块的全局变量 */
  call_foo();

  getchar();
  return 0;
}

