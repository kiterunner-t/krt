CC      = gcc 
RM      = del
CFLAGS  = # /EHsc /I c:\boost
RMFLAGS = 

float: float.c
    $(CC) -O2 $? -o $@
    
cast: cast.c bit.c
    $(CC) $(CFLAGS) $? -o $@

bit_ex: bit_extension.c bit.c
    $(CC) $(CFLAGS) $? -o $@
    
clean:
    $(RM) $(RMFLAGS) *.exe *.obj
