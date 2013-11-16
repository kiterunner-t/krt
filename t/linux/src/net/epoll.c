/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "kmisc.h"


int
tcp_listen(short port)
{
  int                fd;
  int                reuse = 1;
  struct sockaddr_in addr;

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    return KERROR;

  (void) setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    close(fd);
    return -1;
  }

  if (listen(fd, 5) < 0) {
    close(fd);
    return -1;
  }

  return fd;
}


void
epoll_op(int fdep, int op, int fd, int events)
{
  struct epoll_event ev;

  memset(&ev, 0, sizeof(ev));
  ev.data.fd = fd;
  ev.events = events;
  if (epoll_ctl(fdep, op, fd, &ev) < 0)
    kerror("epoll error");
}


int
main(int argc, char *argv[])
{
  int                fd_listen;
  int                fd_ep;
  int                fd_accept;
  int                fd;
  struct epoll_event events[10];
  int                i;
  int                ret;
  int                nums;
  char               line[1024];

  signal(SIGPIPE, SIG_IGN);
  fd_listen = tcp_listen(9999);
  if (fd_listen < 0)
    kerror("listen fd error");

  fd_ep = epoll_create1(EPOLL_CLOEXEC);
  if (fd_ep < 0)
    kerror("epoll error");

  epoll_op(fd_ep, EPOLL_CTL_ADD, fd_listen, EPOLLIN | EPOLLET);
  printf("listen fd: %d\n", fd_listen);
  for ( ; ; ) {
    nums = epoll_wait(fd_ep, events, 10, -1);
    if (nums < 0)
      kerror("epoll wait error");

    for (i = 0; i < nums; ++i) {
      fd = events[i].data.fd;
      if (fd == fd_listen) {
        fd_accept = accept4(fd_listen, NULL, 0, SOCK_NONBLOCK | SOCK_CLOEXEC);
        if (fd_accept < 0)
          kerror("accept error");

        epoll_op(fd_ep, EPOLL_CTL_ADD, fd_accept, EPOLLIN | EPOLLET);

      } else {
        ret = read(fd, line, 5);
        if (ret < 0) {
          if (errno==EINTR || errno==EAGAIN) {
            fprintf(stderr, "read error %d[%s]\n", errno, strerror(errno));
            continue;
          }

          kerror("read error");

        } else if (ret == 0) {
          printf("peer closed");
          epoll_op(fd_ep, EPOLL_CTL_DEL, fd, EPOLLET);
          close(fd);

        } else {
          line[5] = 0;
          printf("read line: %s\n", line);
          if (write(fd, "hi", 3) < 0) {
            if (errno!=EINTR && errno!=EPIPE)
              kerror("write error %d[%s]\n", errno, strerror(errno));
            else
              printf("signal\n");
          }
        }
      }
    }
  }

  return EXIT_SUCCESS;
}

