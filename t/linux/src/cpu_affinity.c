#include <sched.h>
#include <pthread.h>


void
set_process_to_cpu(int cpuid)
{
  cpu_set_t mask;
  cpu_set_t get;

  CPU_ZERO(&mask);
  CPU_SET(cpuid, &mask);

  if (shced_setaffinity(0, sizeof(mask), &mask) < 0)
    error("set process affinity failed\n");

  CPU_ZERO(&get);
  if (sched_getaffinity(0, sizeof(get), &get) < 0)
    error("get process affinity failed\n");
}


void
set_thread_to_cpu(int cpuid)
{
  cpu_set_t mask;
  cpu_set_t get;

  CPU_ZERO(&mask);
  CPU_SET(cpuid, &mask);

  if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    error("set thread affinity failed\n");

  CPU_ZERO(&get);
  if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0)
    error("get thread affinity failed\n");
}


// in x86
#ifdef SMP
# define SMP_LOCK "lock;"
#else
# define SMP_LOCK
#end


static inline int
atomic_cmp_set(volatile atomic_t *dst, uint32_t exp, uint32_t src)
{
  uint8_t res;

  __asm__ volatile(
    SMP_LOCK
    "cmpxchgl %[src], %[dst];"
    "sete %[res];"

    : "=a" (res), [dst] "=m" (src)
    : "r" (src), "a" (exp), "m" (*dst)
    : "memory"
  );

  return res;
}


void
stack_push(stack *s, node *n)
{
  node *h;

  do {
    h = s->head;
    n->next = h;
  } while (!atomic_compare_exchange(s->head, h, n));
}



