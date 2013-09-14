/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <syslog.h>

#define LOG_KDUMP    (0x10000000 | LOG_ERR)
#define LOG_KEXIT    (0x20000000 | LOG_ERR)
#define LOG_KMASK   0x0FFFFFFF

inline void log_init(int is_daemon, const char *proc_name);
inline void log_exit();
inline void log4c(int level, const char *fmt, ...);

#endif

