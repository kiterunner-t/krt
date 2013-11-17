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


// from kernel 2.6.32 linux/inotify.h
const char *g_event_desc[] = {
  "File was accessed",
  "File was modified",
  "Metadata changed",
  "Writtable file was closed",

  "Unwrittable file closed",
  "File was opened",
  "File was moved from X",
  "File was moved to Y",

  "Subfile was created",
  "Subfile was deleted",
  "Self was deleted",
  "Self was moved",

  NULL,
  "Backing fs was unmounted",
  "Event queued overflowed",
  "File was ignored",

  "only watch the path if it is a directory",
  "don't follow a sym link",
  NULL,
  NULL,

  NULL,
  NULL,
  NULL,
  NULL,

  NULL,
  NULL,
  NULL,
  NULL,

  NULL,
  "add to the mask of an already existing watch",
  "event occurred against dir",
  "only send event once"
};


int
main(int argc, char **argv)
{
  char             buf[sizeof(inotify_event_u)*1024];
  inotify_event_u *event;
  int              fd;
  int              wd;
  int              n;
  int              j;

  fd = inotify_init1(IN_CLOEXEC /* IN_NONBLOCK */);
  wd = inotify_add_watch(fd, "./src", IN_ALL_EVENTS);

  for ( ; ; ) {
    n = read(fd, buf, sizeof(buf));
    if (n<0 || n<(sizeof(struct inotify_event)))
      kerror("read error: %s", strerror(errno));

    int len = 0;
    printf("(n is %d) ", n);
    for ( ; len < n; ) {
      event = (inotify_event_u *) buf + len;
      len += sizeof(struct inotify_event) + event->event.len;
      if (event->event.wd != wd) {
        printf("<unkown wd: %d>\n", event->event.wd);
        continue;
      }

      if (event->event.len > 0)
        printf("%s: ", event->event.name);
      else
        printf("%s: ", "./src");

      uint32_t mask = event->event.mask;
      printf("[wd: %d, mask: 0x%x, cookie: %d, len: %d] --> ", 
            event->event.wd, mask, event->event.cookie, event->event.len);

      for (j = 0; j < 32; ++j) {
        if (g_event_desc[j]!=NULL && (mask&(1<<j)))
          printf("%s, ", g_event_desc[j]);
      }

      printf("[%ld]\n", sizeof(struct inotify_event) + event->event.len);
    }

    printf("\n");
  }

  return EXIT_SUCCESS;
}

