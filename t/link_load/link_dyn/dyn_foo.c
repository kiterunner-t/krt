extern void foo_extern();
static void foo_static();
void        foo_internal();

/*
 * type 1: 模块内部的函数调用、跳转
 * type 2: 模块内部的数据访问
 * type 3: 模块外部的数据访问
 * type 4: 模块外部的函数调用、跳转
 */

extern int  g_extern;
int         g_internal;
static int  g_static;

static int  a;
static int *p = &a;  /* 7.3.5 数据段地址无关性 */


static void
foo_static(void)
{
  g_static = 10;   /* type 2 */
  g_internal = 11; /* type 3, 全局对象介入 */
  g_extern = 12;   /* type 3 */
}


void
foo_internal()
{
  *p = 2;
}


void
call_foo()
{
  foo_static();   /* type 1 */
  foo_internal(); /* type 4, 全局对象介入 */
  foo_extern();   /* type 4 */
}

