#ifndef BIT_H_
#define BIT_H_

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, int len);


void show_int(int x);
void show_float(float x);
void show_pointer(void *x);

/* ^ 运算的环属性 */
void inplace_swap(int *x, int *y);

/* mask: x & 0xFF */

/* 
 * x的最低有效字节不变, 其余字节为1: x | ~0xFF
 * x的最低有效字节为0, 其余字节保持不变: x & ~0xFF
 * x的最低有效字节的补, 其余字节保持不变: x ^ 0xFF
 * 
 * 我的解答分别是: x | ((~0)^0xFF), x & ((~0)^0xFF), 分明受到了原文~0的蛊惑.
 */

/* Bit set */
int bis(int x, int m);

/* Bit clear */
int bic(int x, int m);


/* 逻辑运算 ||, &&, ! */
/* a && 5/a, p && *p++ */

#endif /* BIT_H_ */

