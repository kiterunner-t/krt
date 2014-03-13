#include <stdio.h>
#include <stdlib.h>
#include "bit.h"

void
show_bytes(byte_pointer start, int len)
{
    int i;
    
    for (i = 0; i < len; i++)
        printf(" %.2x", start[i]);
    printf("\n");
}

void
show_int(int x)
{
    show_bytes((byte_pointer) &x, sizeof(int));
}

void
show_float(float x)
{
    show_bytes((byte_pointer) &x, sizeof(float));
}

void
show_pointer(void *x)
{
    show_bytes((byte_pointer) &x, sizeof(void *));
}

/* ^ 运算的环属性 */
void
inplace_swap(int *x, int *y)
{
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}

/* mask: x & 0xFF */

/* 
 * x的最低有效字节不变, 其余字节为1: x | ~0xFF
 * x的最低有效字节为0, 其余字节保持不变: x & ~0xFF
 * x的最低有效字节的补, 其余字节保持不变: x ^ 0xFF
 * 
 * 我的解答分别是: x | ((~0)^0xFF), x & ((~0)^0xFF), 分明受到了原文~0的蛊惑.
 */

/* Bit set */
int
bis(int x, int m)
{
    int result = x | m;
    return result;
}

/* Bit clear */
int
bic(int x, int m)
{
    int result = x & ~m;
    return result;
}


/* 逻辑运算 ||, &&, ! */
/* a && 5/a, p && *p++ */
