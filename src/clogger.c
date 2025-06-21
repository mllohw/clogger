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

void clog_msg_with_verb_ctrl(struct clogger_conf conf, const char *file_name,
		const char *func_name, int line_num, int verbosity,
		const char *msg_type, const char *message, ...)
{
	if(verbosity <= conf.verbosity) {
		va_list args;

		// Calculate the needed size
		va_start(args,message);
		size_t bufsz = vformat(NULL,conf.fmt_str,file_name,func_name,
				line_num,conf.app_name,msg_type,
				conf.datetime_fmt,message,args);
		va_end(args);

		// Store the processed format string
		va_start(args,message);
		char *msg = malloc(bufsz*sizeof(char));
		vformat(msg,conf.fmt_str,file_name,func_name,
				line_num,conf.app_name,msg_type,
				conf.datetime_fmt,message,args);
		va_end(args);

		// Print the message to stdout
		fprintf(stdout,"%s",msg);

		// Log the message
		FILE *log_file_ptr = fopen(conf.log_file,"a");
		fprintf(log_file_ptr,"%s",msg);
		fclose(log_file_ptr);

		free(msg);
	}
}
