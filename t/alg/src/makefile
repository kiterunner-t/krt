CC       = gcc
CC_FLAGS = -Wall -g -I ../../../lib/include -I ../include -L ../../../lib
BINDIR   = ../bin


ALL_OBJS = test_redblack_tree \
           test_skip_list_deterministic \
           test_splay_tree \
           test_aa_tree \
           test_treap_tree

ITEM_SRC = kitem_long.c kitem_string.c


% :
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $^ $(ITEM_SRC) -lk


all: $(ALL_OBJS)
	echo ""


test_redblack_tree: test_redblack_tree.c redblack_tree.c

test_skip_list_deterministic: test_skip_list_deterministic.c \
    skip_list_deterministic.c

test_splay_tree: test_splay_tree.c splay_tree_topdown.c

test_aa_tree: test_aa_tree.c aa_tree.c

test_treap_tree: test_treap_tree.c treap_tree.c


clean:
	-rm -f *.o $(addprefix $(BINDIR)/, $(ALL_OBJS))

