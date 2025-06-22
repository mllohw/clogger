/**  Logging functions for clogger
 *
 *   Copyright (C) 2025  Mirko Lindroth
 *   SPDX-License-Identifier: MIT
 */
#include <clogger/formatter.h>
#include <clogger/clogger.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define format_message(format) \
	va_start(args,message);\
	bufsz = vformat(NULL,format,file_name,\
			func_name,line_num,conf.app_name,msg_type,\
			conf.datetime_fmt,message,args);\
	va_end(args);\
	va_start(args,message);\
	msg = realloc(msg,bufsz*sizeof(char));\
	vformat(msg,format,file_name,func_name,\
			line_num,conf.app_name,msg_type,\
			conf.datetime_fmt,message,args);\
	va_end(args);


void clog_msg_with_verb_ctrl(struct clogger_conf conf, const char *file_name,
		const char *func_name, int line_num, int verbosity,
		const char *msg_type, const char *message, ...)
{
	if(verbosity <= conf.verbosity) {
		va_list args;
		size_t bufsz;
		char *msg = (char*)malloc(sizeof(char));

		// Print the message to stdout
		format_message(conf.fmt.console_format)
		fprintf(stdout,"%s",msg);

		// Log the message
		format_message(conf.fmt.log_file_format)
		FILE *log_file_ptr = fopen(conf.log_file,"a");
		fprintf(log_file_ptr,"%s",msg);
		fclose(log_file_ptr);

		free(msg);
	}
}
