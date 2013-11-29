VPATH = ../alg/src ../alg/include
CC = gcc
CCFLAGS = -g -Wall -I ../../lib/include -I ../alg/include -D KUSE_GCC_ATOMIC -D KITEM_LONG


all: t_lock_no lock_pthread_mutex lock_peterson lock_tas lock_ttas


%: %.c
	$(CC) $(CCFLAGS) -o bin/$@ $^ -lpthread


t_lock_no: thread.c t_lock_no.c

lock_pthread_mutex: lock_pthread_mutex.c thread.c t_lock.c

lock_peterson2: lock_peterson.c thread.c t_lock.c
	$(CC) -g -Wall -D KUSE_GCC_ATOMIC -I ../../lib/include -I ../alg/include -o bin/$@ $^ -lpthread

lock_peterson: lock_peterson.c thread.c t_lock.c
	$(CC) -g -Wall -I ../../lib/include -I ../alg/include -o bin/$@ $^ -lpthread


lock_tas: lock_tas.c thread.c t_lock.c

lock_ttas: lock_ttas.c thread.c t_lock.c

lock_anderson: lock_anderson.c thread.c t_lock.c
	$(CC) $(CCFLAGS) -D LOCK_A -D KUSE_GCC_ATOMIC -o bin/$@ $^ -lpthread

lock_anderson_cacheline: lock_anderson.c thread.c t_lock.c
	$(CC) $(CCFLAGS) -D LOCK_A -D CACHELINE=$(shell getconf LEVEL1_DCACHE_LINESIZE) -D KUSE_GCC_ATOMIC -o bin/$@ $^ -lpthread


list_coarse_lock: t_list.c list_coarse_lock.c thread.c lock_pthread_mutex.c kitem_long.c

list_fine_lock: t_list.c list_fine_lock.c thread.c lock_pthread_mutex.c kitem_long.c

list_optimistic_lock: t_list.c list_optimistic_lock.c thread.c lock_pthread_mutex.c kitem_long.c

ut_list: ut_list.c list_optimistic_lock.c thread.c lock_pthread_mutex.c kitem_long.c

