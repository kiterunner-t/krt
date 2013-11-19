CC = gcc
CCFLAGS = -g -Wall -I ../../lib/include


all: t_lock_no lock_pthread_mutex lock_peterson lock_tas lock_ttas


%: %.c
	$(CC) $(CCFLAGS) -o bin/$@ $^ -lpthread


t_lock_no: thread.c t_lock_no.c

lock_pthread_mutex: lock_pthread_mutex.c thread.c t_lock.c

lock_peterson: lock_peterson.c thread.c t_lock.c

lock_tas: lock_tas.c thread.c t_lock.c

lock_ttas: lock_ttas.c thread.c t_lock.c

lock_a: lock_a.c thread.c t_lock.c
	$(CC) $(CCFLAGS) -D LOCK_A -o bin/$@ $^ -lpthread

lock_a_cacheline: lock_a.c thread.c t_lock.c
	$(CC) $(CCFLAGS) -D LOCK_A -D CACHELINE=$(shell getconf LEVEL1_DCACHE_LINESIZE) -o bin/$@ $^ -lpthread

