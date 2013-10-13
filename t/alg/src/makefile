CC       = gcc
CC_FLAGS = -Wall -g -I../../../lib/include -I../include
BINDIR   = ../bin

ALL_OBJS = rb dsl avl bh sort
ITEM_SRC = item_long.c item_long.h item_string.c item_string.h

%.o : %.c
	$(CC) $(CC_FLAGS) $<


all: $(ALL_OBJS)


rb: test_redblack_tree.c redblack_tree.c redblack_tree.h $(ITEM_SRC)
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


dsl: test_skip_list_deterministic.c skip_list_deterministic.c item_long.c item_string.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


test_splay_tree: test_splay_tree.c splay_tree_topdown.c item_long.c item_string.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


avl: test_avl_tree.c avl_tree.c avl_tree.h item_long.c item_string.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


bh: test_binary_heap.c binary_heap.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


sort: test_sort.c sort.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^


clean:
	-rm -f *.o $(addprefix $(BINDIR)/, $(ALL_OBJS))

