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
#define DEBUG 500

struct log_format {
	char *console_format;
	char *log_file_format;
};

struct clogger_conf {
	int verbosity;
	char *app_name;
	char *log_file;
	struct log_format debug_fmt;
	struct log_format info_fmt;
	struct log_format warning_fmt;
	struct log_format error_fmt;
	struct log_format fatal_fmt;
	char *datetime_fmt;
};

// Console and file logging functions
void clog_msg_with_verb_ctrl(struct clogger_conf conf, const char *file_name,
		const char *func_name, int line_num, int verbosity,
		const char *msg_type, struct log_format fmt,
		const char *message, ...);

// Reporting macros for different message types
#define debug(conf,msg,...) \
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,DEBUG,"DEBUG",\
			conf.debug_fmt,msg,##__VA_ARGS__)

#define info(conf,verb,msg,...) \
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,verb,"INFO",\
			conf.info_fmt,msg,##__VA_ARGS__)

#define warning(conf,msg,...) \
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,NONE,"WARNING",\
			conf.warning_fmt,msg,##__VA_ARGS__)

#define error(conf,msg,...) \
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,NONE,"ERROR",\
			conf.error_fmt,msg,##__VA_ARGS__)

#define fatal(conf,msg,...) \
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,NONE,"FATAL",\
			conf.fatal_fmt,msg,##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
