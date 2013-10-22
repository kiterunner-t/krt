VPATH     = src include ../../lib/src ../../lib/include
BINDIR    = bin

CC        = gcc
CC_FLAGS  = -Wall -g -L ../../lib
CPP_FLAGS = -I ../../lib/include -I include


ALL_OBJS  = test_redblack_tree \
            test_skip_list_deterministic \
            test_splay_tree \
            test_aa_tree \
            test_treap_tree \
            test_avl_tree

ITEM_SRC = kitem_long.c kitem_string.c


% :
	$(CC) $(CC_FLAGS) $(CPP_FLAGS) -o $(BINDIR)/$@ $^ -lk


all: $(ALL_OBJS) TAGS
	@echo ""


TAGS: src/*.[ch] include/*.h ../../lib/src/*.[ch] ../../lib/include/*.h
	@rm -f $@
	find . -name "*.[ch]" | xargs etags -R -a
	find ../../lib -name "*.[ch]" | xargs etags -R -a


test_redblack_tree: test_redblack_tree.c redblack_tree.c $(ITEM_SRC)

test_skip_list_deterministic: test_skip_list_deterministic.c \
    skip_list_deterministic.c  $(ITEM_SRC)

test_splay_tree: test_splay_tree.c splay_tree_topdown.c  $(ITEM_SRC)

test_aa_tree: test_aa_tree.c aa_tree.c  $(ITEM_SRC)

test_treap_tree: test_treap_tree.c treap_tree.c  $(ITEM_SRC)

test_avl_tree: test_avl_tree.c avl_tree.c  $(ITEM_SRC)


clean:
	-rm -f *.o $(addprefix $(BINDIR)/, $(ALL_OBJS))

