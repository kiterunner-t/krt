CC       = gcc
CC_FLAGS = -Wall -g -I include -I ../../lib/include
LD_FLAGS = -lrt -lpthread
BINDIR   = bin
VPATH    = src src/net src/sig src/io src/time src/thread include ../../lib/include


%: %.c
	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $(filter %.c,$^) $(LD_FLAGS)


all: epoll signalfd timerfd test_aio inotify


test_aio: test_aio.c kaio.h


test_pipeline: test_pipeline.c pipeline.c pipeline.h
#	$(CC) $(CC_FLAGS) -o $(BINDIR)/$@ $(filter %.c,$^) $(LD_FLAGS)

