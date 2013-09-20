CC       = gcc
CC_FLAGS = -g -I../../../lib/include -I../include
BINDIR   = ../bin

ALL_OBJS = bh sort rb dsl


all: $(ALL_OBJS)


bh: test_binary_heap.c binary_heap.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^
	$(BINDIR)/$@


sort: test_sort.c sort.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^
	$(BINDIR)/$@


rb: test_redblack_tree.c redblack_tree.c item_long.c item_string.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


dsl: test_skip_list_deterministic.c skip_list_deterministic.c item_long.c item_string.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


clean:
	-rm -f *.o $(addprefix $(BINDIR)/, $(ALL_OBJS))

