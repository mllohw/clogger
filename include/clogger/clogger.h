/**  Header for clogger logging
 *
 *   Copyright (C) 2025  Mirko Lindroth
 *   SPDX-License-Identifier: MIT
 */
#ifndef CLOGGER_H
#define CLOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

// Return macro's name
#define MACRO_NAME(x) #x

#define NONE 0
#define LOW 100
#define AVG 200
#define HIGH 300
#define FULL 400

struct log_formats {
	char *console_format;
	char *log_file_format;
};

struct clogger_conf {
	int verbosity;
	char *app_name;
	char *log_file;
	struct log_formats fmt;
	char *datetime_fmt;
};

// Console and file logging functions
void clog_msg_with_verb_ctrl(struct clogger_conf conf, const char *file_name,
		const char *func_name, int line_num, int verbosity,
		const char *msg_type, const char *message, ...);

// Reporting macros for different message types
#define info(conf,verb,fmt_str,...) \
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,verb,"INFO",\
			fmt_str,##__VA_ARGS__)
#ifdef __cplusplus
}
#endif

#endif
