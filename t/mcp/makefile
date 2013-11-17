CC = gcc
CCFLAGS = -g -Wall -I ../../lib/include


all: t_lock_peterson t_lock_tas t_lock_ttas


t_lock_peterson: lock_peterson.c thread.c t_lock.c
	$(CC) $(CCFLAGS) -o bin/$@ $^ -lpthread


t_lock_tas: lock_tas.c thread.c t_lock.c
	$(CC) $(CCFLAGS) -o bin/$@ $^ -lpthread


t_lock_ttas: lock_ttas.c thread.c t_lock.c
	$(CC) $(CCFLAGS) -o bin/$@ $^ -lpthread

