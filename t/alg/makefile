VPATH     = src t include ../../lib/src ../../lib/include
BINDIR    = bin
MKDIR     = bin/mk
OBJSDIR   = objs
CC        = gcc
LD        = ld
CCFLAGS   = -Wall -g
CPPFLAGS  = -I ../../lib/include -I include
LDFLAGS   = -L ../../lib
LDLIBS    = -lk

ALL_BINS  = test_redblack_tree \
            test_skip_list_deterministic \
            test_splay_tree_topdown \
            test_aa_tree \
            test_avl_tree \
            test_kd_tree \
            test_binary_heap
                        #test_treap_tree \            test_st_bst \

vpath %.c t
vpath %.c src
vpath %.o $(OBJSDIR)
vpath %.o $(OBJSDIR)/t
vpath %.o $(OBJSDIR)/src


define make_depend
	$(CC) -MM $(CPPFLAGS) $1 >$(MKDIR)/$2.mk__
	-@perl mk_convert.pl $(MKDIR)/$2.mk__
	-@rm -f $(MKDIR)/$2.mk__
endef


define make_rule
  $1: objs/t/$1.o objs/src/$(subst test_,,$1).o objs/src/kitem_long.o objs/src/kitem_string.o
	$(CC) $(LDFLAGS) $(LDLIBS) -o $(BINDIR)/$$@ $$^

  objs/%.o: %.c
	$(CC) $(CCFLAGS) $(CPPFLAGS) -o $$@ -c $$(filter %.c,$$^)

  $(MKDIR)/$1.mk: t/$1.c src/$(subst test_,,$1).c src/kitem_long.c src/kitem_string.c
	$(call make_depend,$$^,$1)

  -include $(MKDIR)/$1.mk
endef


all: $(ALL_BINS) TAGS


TAGS: src/*.[ch] t/*.[ch] include/*.h ../../lib/src/*.[ch] ../../lib/include/*.h
	@rm -f $@
	find . ../../lib -name "*.[ch]" | xargs etags -R -a


clean:
	-rm -f $(addprefix $(BINDIR)/,$(ALL_BINS)) $(addprefix $(BINDIR)/mk/,$(addsuffix .mk,$(ALL_BINS)))
	-find . -name *.o | xargs rm -f


$(if $(filter $(MAKECMDGOALS),clean TAGS all),,$(eval $(call make_rule,$(MAKECMDGOALS))))

ifeq "$(MAKECMDGOALS)" "all"
  $(foreach obj,$(ALL_BINS),$(eval $(call make_rule,$(obj))))
endif

