/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include "kmisc.h"


static void print_elapsed_time(void);


int
main(int argc, char *argv[])
{
  struct itimerspec new_time;
  struct itimerspec old_time;
  int               max_exp;
  int               fd;
  int               i;
  uint64_t          cnt;
  uint64_t          total_cnt = 0;
  int               n;

  if (argc!=2 && argc!=4) {
    printf("Usage: %s init-secs [interval-secs max-exp]\n", argv[0]);
    return EXIT_FAILURE;
  }

  memset(&new_time, 0, sizeof(new_time));
  new_time.it_value.tv_sec = atoi(argv[1]);
  if (argc == 2) {
    max_exp = 1;
  } else {
    new_time.it_interval.tv_sec = atoi(argv[2]);
    max_exp = atoi(argv[3]);
  }

  fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC /* TFD_NONBLOCK */);
  if (fd < 0)
    kerror("timerfd_create error: %s", strerror(errno));

  if (timerfd_settime(fd, 0, &new_time, &old_time) < 0)
    kerror("timerfd_settime error: %s", strerror(errno));

  print_elapsed_time();
  for (i = 0; i < max_exp; ++i) {
    n = read(fd, &cnt, sizeof(cnt));
    if (n < 0)
      kerror("read error: %s", strerror(errno));

    total_cnt += cnt;
    print_elapsed_time();
    printf("    total_cnt is: %ld %ld\n", total_cnt, cnt);
  }

  return EXIT_SUCCESS;
}


static void
print_elapsed_time(void)
{
  static struct timespec start;
  static struct timespec cur;
  static kbool_t         is_first_call = KTRUE;
  int                    secs;
  int                    nsecs;

  if (is_first_call == KTRUE) {
    is_first_call = KFALSE;
    clock_gettime(CLOCK_MONOTONIC, &start);
  }

  clock_gettime(CLOCK_MONOTONIC, &cur);
  secs = cur.tv_sec - start.tv_sec;
  nsecs = cur.tv_nsec - start.tv_nsec;
  if (nsecs < 0) {
    secs--;
    nsecs += 1000000000;
  }

  printf("%d.%03d: \n", secs, (nsecs + 500000) / 1000000);
}

