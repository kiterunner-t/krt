/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/signalfd.h>
#include "kmisc.h"


int
signalfd_init()
{
  sigset_t mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGQUIT);

  if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
    return -1;

  return signalfd(-1, &mask, /* SFD_NONBLOCK | */ SFD_CLOEXEC);
}


int
main(int argc, char *argv[])
{
  struct signalfd_siginfo siginfo;
  kbool_t                 is_quit = KFALSE;
  int                     sfd;
  int                     sig;
  int                     n;

  sfd = signalfd_init();
  if (sfd < 0)
    kerror("signalfd_init error: %s", strerror(errno));

  sleep(5);
  while (is_quit == KFALSE) {
    n = read(sfd, &siginfo, sizeof(siginfo));
    if (n < 0) {
      if (errno!=EAGAIN && errno!=EINTR)
        kerror("read error: %s", strerror(errno));
      else
        fprintf(stderr, "read again\n");

    } else if (n == sizeof(siginfo)) {
      sig = siginfo.ssi_signo;
      if (sig == SIGINT)
        printf("caught SIGINT\n");
      else if (sig == SIGQUIT)
        is_quit = KTRUE;
      else
        printf("unkown signo: %d\n", sig);

    } else {
      fprintf(stderr, "read error: %d\n", n);
      close(sfd);
      is_quit = KFALSE;
    }
  }

  return EXIT_SUCCESS;
}

