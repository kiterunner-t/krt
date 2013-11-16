#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "sig.h"


int
sig_mask_print(void)
{
  sigset_t mask;
  int      errno_saved;
  int      i;

  errno_saved = errno;
  if (sigprocmask(0, NULL, &mask) < 0)
    return -1;

  for (i = 1; i < NSIG; ++i) {
    if (sigismember(&mask, i))
      printf("%s ", strsignal(i));
  }
  printf("\n");

  errno = errno_saved;
  return 0;
}


sig_handler_pt
sig_sethandler(int signo, sig_handler_pt func)
{
  struct sigaction act;
  struct sigaction oact;

  memset(&act, 0, sizeof(act));
  sigemptyset(&act.sa_mask);

  act.sa_handler = func;
  if (signo == SIGALRM)
    act.sa_flags |= SA_INTERRUPT;
  else
    act.sa_flags |= SA_RESTART;

  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;

  return oact.sa_handler;
}

