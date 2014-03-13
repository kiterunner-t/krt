#include <stdio.h>
#include <stdlib.h>

double
recip(int denom)
{
    return 1.0 / (double) denom;
}

void do_nothing() { } /* Just like the name says */

void
test1(int denom)
{
    double r1, r2;
    int t1, t2;
    
    r1 = recip(denom); /* Stored in memory */
    r2 = recip(denom); /* Stored in register */
    t1 = r1 == r2;     /* Compares register to memory */
    do_nothing();      /* Forces register save to memory */
    t2 = r1 == r2;     /* Compares memory to memory */
    printf("test1 t1: r1 %f %c= r2 %f\n", r1, t1 ? '=' : '!', r2);
    printf("test1 t2: r1 %f %c= r2 %f\n", r1, t2 ? '=' : '!', r2);
}

void
test2(int denom)
{
    double r1;
    int t1;
    
    r1 = recip(denom);
    t1 = r1 == 1.0 / (double) denom;
    printf("test2 t1: r1 %f %c= 1.0/10.0\n", r1, t1 ? '=' : '!');
}

long double recip_1(int denom)
{
    return 1.0 / (long double) denom;
}

void
test3(int denom)
{
    long double r1, r2;
    int t1, t2, t3;
    
    r1 = recip_1(denom);
    r2 = recip_1(denom); /* Stored in register */
    t1 = r1 == r2;     /* Compares register to memory */
    do_nothing();      /* Forces register save to memory */
    t2 = r1 == r2;     /* Compares memory to memory */
    t3 = r1 == 1.0 / (long double) denom;
    printf("test3 t1: r1 %f %c= r2 %f\n", (double) r1, t1 ? '=' : '!', (double) r2);
    printf("test3 t2: r1 %f %c= r2 %f\n", (double) r1, t2 ? '=' : '!', (double) r2);
    printf("test3 t3: r1 %f %c= 1.0/10.0\n", (double) r1, t2 ? '=' : '!');
}

int
main(void)
{
    test1(10);
    test2(10);
    test3(10);
    
    return EXIT_SUCCESS;
}
