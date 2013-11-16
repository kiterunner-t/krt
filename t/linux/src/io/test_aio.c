#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define __USE_GNU 1
#include <fcntl.h>
#include <inttypes.h>
#include <linux/aio_abi.h>
#include "kmisc.h"
#include "kaio.h"


int
main(int argc, char **argv)
{
  aio_context_t    ctx;
  struct iocb      cb;
  struct iocb     *cbs[1];
  struct io_event  events[1];
  char             data[4096];
  int              ret;
  int              fd;

  fd = open("/tmp/testfile", O_DIRECT | O_RDWR | O_CREAT);
  if (fd < 0)
    kerror("open error");

  memset(&ctx, 0, sizeof(ctx));
  ret = io_setup(128, &ctx);
  if (ret < 0)
    kerror("io_setup error");

  memset(&cb, 0, sizeof(cb));
  cb.aio_fildes = fd;
  cb.aio_lio_opcode = IOCB_CMD_PWRITE;
  cb.aio_buf = (uint64_t) data;
  cb.aio_offset = 0;
  cb.aio_nbytes = 4096;

  cbs[0] = &cb;

  ret = io_submit(ctx, 1, cbs);
  if (ret < 0)
    kerror("io_submit error");
  else if (ret != 1)
    kerror("could not submit IO");

  ret = io_getevents(ctx, 1, 1, events, NULL);
  printf("aio get events: %d\n", ret);

  if (io_destroy(ctx) < 0)
    kerror("io_destroy error");

  return 0;
}

