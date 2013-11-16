/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/inotify.h>
#include "kmisc.h"


typedef union inotify_event_s inotify_event_u;

union inotify_event_s {
  char                 buf[sizeof(struct inotify_event)+NAME_MAX+1];
  struct inotify_event event;
};


int
main(int argc, char **argv)
{
  char             buf[sizeof(inotify_event_u)*1024];
  inotify_event_u *event;
  int              fd;
  int              wd;
  int              n;

  fd = inotify_init1(IN_CLOEXEC /* IN_NONBLOCK */);
  wd = inotify_add_watch(fd, "./src/test.txt", IN_ALL_EVENTS);

  for ( ; ; ) {
    n = read(fd, buf, sizeof(buf));
    if (n < sizeof(struct inotify_event))
      kerror("read error: %s", strerror(errno));

    int len = 0;
    printf("(n is %d) ", n);
    for ( ; len < n; ) {
      event = (inotify_event_u *) buf + len;
      if (event->event.len > 0)
        printf("%s: ", event->event.name);
      else
        printf("%s: ", "./src/test.txt");

      uint32_t mask = event->event.mask;
      printf("[%d, 0x%x, %d] --> ", event->event.wd, mask, event->event.cookie);
      if (mask & IN_ACCESS)
        printf("IN_ACCESS");
      else if (mask & IN_ATTRIB)
        printf("IN_ATTRIB");
      else if (mask & IN_CLOSE_WRITE)
        printf("IN_CLOSE_WRITE");
      else if (mask & IN_CLOSE_NOWRITE)
        printf("IN_CLOSE_NOWRITE");
      else if (mask & IN_CREATE)
        printf("IN_CREATE");
      else if (mask & IN_DELETE)
        printf("IN_DELETE");
      else if (mask & IN_DELETE_SELF)
        printf("IN_DELETE_SELF");
      else if (mask & IN_MODIFY)
        printf("IN_MODIFY");
      else if (mask & IN_MOVE_SELF)
        printf("IN_MOVE_SELF");
      else if (mask & IN_MOVED_FROM)
        printf("IN_MOVED_FROM");
      else if (mask & IN_MOVED_TO)
        printf("IN_MOVED_TO");
      else if (mask & IN_OPEN)
        printf("IN_OPEN");
      else if (mask & IN_IGNORED)
        printf("IN_IGNORED");
      else if (mask & IN_ISDIR)
        printf("IN_ISDIR");
      else if (mask & IN_Q_OVERFLOW)
        printf("IN_Q_OVERFLOW");
      else if (mask & IN_UNMOUNT)
        printf("IN_UNMOUNT");
      else
        printf("unkown");

      printf("[%ld]\n", sizeof(struct inotify_event) + event->event.len);
      len += sizeof(struct inotify_event) + event->event.len;
    }

    printf("\n");
  }

  return EXIT_SUCCESS;
}

