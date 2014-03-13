#include <stdio.h>
#include <stdlib.h>
#include "bit.h"

int
fun1(unsigned word)
{
    return (int) ((word << 24) >> 24);
}

int
fun2(unsigned word)
{
    return ((int) word << 24) >> 24;
}

void
trunc_number(void)
{
    int x = 53191;
    short sx = (short) x; /* -12345 */
    int y = sx;           /* -12345 */
    
    printf("x = %d: \t", x);
    show_int(x);
    
//    printf("sx = %u: \t", sx);
    printf("y = %d: \t", y);
}

/* WARNING: This is buggy code. */
float
sum_elements(float a[], unsigned length)
{
    int i;
    float result = 0;
    
    for (i = 0; i <= length-1; i++)
        result += a[i];
    
    return result;
}

int
main(void)
{
    short sx = -12345;
    unsigned short usx = sx; /* 53191 */
    int x = sx;              /* -12345 */
    unsigned ux = usx;       /* 53191 */
    
    unsigned uy = x;
    
    unsigned word = 256;
    int a = fun1(word);
    int b = fun2(word);
    
    printf("sx = %d:\t", sx);
    show_bytes((byte_pointer) &sx, sizeof(short));
    
    printf("usx = %u:\t", usx);
    show_bytes((byte_pointer) &usx, sizeof(unsigned short));
    
    printf("x = %d:\t", x);
    show_bytes((byte_pointer) &x, sizeof(int));
    
    /* expression: (unsigned) (unsigned short) sx; -> zero extension*/
    printf("ux = %u:\t", ux);
    show_bytes((byte_pointer) &ux, sizeof(unsigned));
    
    printf("-------------------------------------\n\n");
    
    /* expression: (unsigned) (int) sx; -> sign extension */
    printf("uy = %u:\t", uy);
    show_bytes((byte_pointer) &uy, sizeof(unsigned));
    
    printf("-------------------------------------\n\n");
    
    printf("a = %d:\t", a);
    show_int(a);
    
    printf("b = %d:\t", b);
    show_int(b);
    
    printf("\n\n-------------------------------------\n");
    trunc_number();

    return EXIT_SUCCESS;
}
    