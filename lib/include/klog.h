/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef KLOG_H_
#define KLOG_H_

#include <stdarg.h>
#include <syslog.h>


#define LOG_KDUMP    (0x10000000 | LOG_ERR)
#define LOG_KEXIT    (0x20000000 | LOG_ERR)
#define LOG_KMASK     0x0FFFFFFF


extern void klog_init(int is_daemon, const char *proc_name);
extern void klog_exit();

#define klog(level, fmt, ...)                                       \
  klog_(level, "file (%s/%s/%d)" fmt "\n",                          \
               __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)


#endif

