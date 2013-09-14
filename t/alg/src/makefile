CC       = gcc
CC_FLAGS = -g -I../../../lib/include -I../include
BINDIR   = ../bin


bh: test_binary_heap.c binary_heap.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^
	$(BINDIR)/$@


sort: test_sort.c sort.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^
	$(BINDIR)/$@


rb: test_redblack_tree.c redblack_tree.c item_long.c item_string.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


clean:
	-rm *.o bh

