#ifndef _SYSLOG_H_
#define _SYSLOG_H_

#include <stdarg.h>


int vsprintf(char *buf, const char *fmt, va_list args);
void k_printf (const char *fmt, ...);


#endif /* _SYSLOG_H_ */
