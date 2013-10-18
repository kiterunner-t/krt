VPATH    = src
CC       = gcc
CC_FLAGS = -Wall -g -I include


%.o : %.c
	$(CC) $(CC_FLAGS) -c -o $@ $^


libk.a: klog.o krand.o
	ar -rv $@ $^
	-rm -f $^

