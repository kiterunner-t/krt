/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <syslog.h>
#include "log.h"

#define LOG_MAXLINE  2048

static int g_is_daemon;

static void _log(int flag, int level, const char *fmt, va_list ap);


inline void
log_init(int d, const char *proc_name)
{
  g_is_daemon = d;

  if (g_is_daemon)
    openlog(proc_name, LOG_PID, LOG_USER);  // todo
}


inline void
log_exit()
{
  if (g_is_daemon)
    closelog();
}


inline void
log4(int level, const char *fmt, ...)
{
  va_list ap;
  int     is_errno = 0;

  if ((level&LOG_KMASK) <= LOG_ERR)
    is_errno = 1;

  va_start(ap, fmt);
  _log(is_errno, level & LOG_KMASK, fmt, ap);
  va_end(ap);

  switch (level) {
    case LOG_KDUMP:
      abort();
      break;

    case LOG_KEXIT:
      exit(1);
      break;

    default:
      break;
  }
}


static void
_log(int errnoflag, int level, const char *fmt, va_list ap)
{
  int  errno_save;
  int  len;
  char buf[LOG_MAXLINE+1];

  errno_save = errno;
  vsnprintf(buf, sizeof(buf) - 1, fmt, ap);

  len = strlen(buf);
  if (errnoflag)
    snprintf(buf + len, sizeof(buf) - len, ": %s", strerror(errno_save));
  strcat(buf, "\n");
  
  if (g_is_daemon) {
    syslog(level, buf);
    return ;
  }

  fflush(stdout);
  fputs(buf, stderr);
  fflush(stderr);
}

