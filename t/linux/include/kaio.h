#ifndef KAIO_H_
#define KAIO_H_

#include <time.h>
#include <sys/syscall.h>
#include <linux/aio_abi.h>

#define __USE_GNU 1


static inline int
io_setup(unsigned nr, aio_context_t *ctxp)
{
  return syscall(__NR_io_setup, nr, ctxp);
}


static inline int
io_destroy(aio_context_t ctx)
{
  return syscall(__NR_io_destroy, ctx);
}


static inline int
io_submit(aio_context_t ctx, long nr, struct iocb **iocbpp)
{
  return syscall(__NR_io_submit, ctx, nr, iocbpp);
}


static inline int
io_cancel(aio_context_t ctx, struct iocb *iocb, struct io_event *result)
{
  return syscall(__NR_io_cancel, ctx, iocb, result);
}


static inline int
io_getevents(aio_context_t ctx, long min_nr, long max_nr,
             struct io_event *events, struct timespec *timeout)
{
  return syscall(__NR_io_getevents, ctx, min_nr, max_nr, events, timeout);
}


#endif

