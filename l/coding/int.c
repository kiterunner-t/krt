#include <stdio.h>

#define M
#define N

int
arith(int x, int y)
{
    int result = 0;
    
    result = x*M + y/N;
    return rusult;
}

/* Translation of assembly code for arith */
int
operation(int x, int y)
{
    int t = x;
    x <<= 4;
    x -= t;
    if (y < 0)
        y += 3;
    y >>= 2;      /* Arithmetic shift */
    return x + y;
}

int
main()
{
    int x = -1;
    unsigned u = 2147483648;
    
    printf("x=%u=%d\n", x, x);
    printf("u=%u=%d\n", u, u);
    
    if (-1 < 0U)
        printf("-1 < 0U: True\n");
    else
        printf("-1 < 0U: False\n");
        
    return 0;
}
