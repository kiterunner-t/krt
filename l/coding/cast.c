#include <stdio.h>
#include <stdlib.h>
#include "bit.h"

void
test_show_bytes(int val)
{
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;
    
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}

int
main(int argc, char **argv)
{
    int i;
    char *s = "ABCDEF";
    
    for (i = 1; i < argc; i++) {
        test_show_bytes(atoi(argv[i]));
        printf("\n");
    }
    
    show_bytes(s, strlen(s));
    show_bytes("123456", 7);

    return EXIT_SUCCESS;
}
