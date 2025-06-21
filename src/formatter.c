/**  Formatting functions for clogger
 *
 *   Copyright (C) 2025  Mirko Lindroth
 *   SPDX-License-Identifier: MIT
 */
#include <clogger/formatter.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

size_t vformat(char *output, const char *format_str, const char *file_name,
		const char *func_name,const int line_num, const char *app,
		const char *msg_type, const char *datetime_fmt,
		const char *message, va_list args)
{
	size_t bufsz,has_token;

	bufsz = strlen(format_str) + 1;
	char *temp_out = (char*)malloc(bufsz*sizeof(char));
	strcpy(temp_out,format_str);

	// Convert line number to string
	char *line_num_str = (char*)malloc((log10(line_num)+1)*sizeof(char));
	sprintf(line_num_str,"%d",line_num);

	// Get datetime str if datetime format is given
	char *dt_out = (char*)malloc(sizeof(char));
	size_t dt_bufsz;
	if(datetime_fmt != NULL) {
		dt_bufsz = format_datetime(NULL,datetime_fmt);
		dt_out = (char*)realloc(dt_out,(dt_bufsz+1)*sizeof(char));
		dt_bufsz = format_datetime(dt_out,datetime_fmt);
	}

	// Replace format specifiers
	has_token = replace_token(NULL,temp_out,"%(app)",app);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(app)",app);
	}
	has_token = replace_token(NULL,temp_out,"%(msg_type)",msg_type);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(msg_type)",msg_type);
	}

	has_token = replace_token(NULL,temp_out,"%(datetime)",dt_out);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(datetime)",dt_out);
	}

	has_token = replace_token(NULL,temp_out,"%(file)",file_name);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(file)",file_name);
	}

	has_token = replace_token(NULL,temp_out,"%(func)",func_name);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(func)",func_name);
	}

	has_token = replace_token(NULL,temp_out,"%(line)",line_num_str);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(line)",line_num_str);
	}

	has_token = replace_token(NULL,temp_out,"%(message)",NULL);
	if(has_token) {

		// Calculate buffer for message formatting
		va_list temp_args;
		va_copy(temp_args,args);
		size_t msg_bufsz = vsnprintf(NULL,0,message,temp_args);

		// Allocate buffer to format the message
		char *temp_msg = malloc((msg_bufsz+1)*sizeof(char));

		// Format message
		vsprintf(temp_msg,message,args);

		// Format output
		bufsz = replace_token(NULL,temp_out,"%(message)",temp_msg);
		temp_out = (char*)realloc(temp_out,(bufsz+1)*sizeof(char));
		replace_token(temp_out,temp_out,"%(message)",temp_msg);
		free(temp_msg);
	}

	if(output != NULL) {strcpy(output,temp_out);}

	free(line_num_str);
	free(temp_out);
	free(dt_out);
	return bufsz;
}

size_t format(char *output, const char *format_str, const char *file_name,
		const char *func_name,const int line_num, const char *app,
		const char *msg_type, const char *datetime_fmt,
		const char *message, ...)
{
	size_t bufsz,has_token;

	bufsz = strlen(format_str) + 1;
	char *temp_out = (char*)malloc(bufsz*sizeof(char));
	strcpy(temp_out,format_str);

	// Convert line number to string
	char *line_num_str = (char*)malloc((log10(line_num)+1)*sizeof(char));
	sprintf(line_num_str,"%d",line_num);

	// Get datetime str if datetime format is given
	char *dt_out = (char*)malloc(sizeof(char));
	size_t dt_bufsz;
	if(datetime_fmt != NULL) {
		dt_bufsz = format_datetime(NULL,datetime_fmt);
		dt_out = (char*)realloc(dt_out,(dt_bufsz+1)*sizeof(char));
		dt_bufsz = format_datetime(dt_out,datetime_fmt);
	}

	// Replace format specifiers
	has_token = replace_token(NULL,temp_out,"%(app)",app);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(app)",app);
	}

	has_token = replace_token(NULL,temp_out,"%(msg_type)",msg_type);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(msg_type)",msg_type);
	}

	has_token = replace_token(NULL,temp_out,"%(datetime)",dt_out);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(datetime)",dt_out);
	}

	// Replace format specifiers
	has_token = replace_token(NULL,temp_out,"%(datetime)",dt_out);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(datetime)",dt_out);
	}

	has_token = replace_token(NULL,temp_out,"%(file)",file_name);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(file)",file_name);
	}

	has_token = replace_token(NULL,temp_out,"%(func)",func_name);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(func)",func_name);
	}

	has_token = replace_token(NULL,temp_out,"%(line)",line_num_str);
	if(has_token) {
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(line)",line_num_str);
	}

	has_token = replace_token(NULL,temp_out,"%(message)",message);
	if(has_token) {
		va_list args;

		// Calculate buffer for message formatting
		va_start(args,message);
		size_t msg_bufsz = vsnprintf(NULL,0,message,args);
		va_end(args);

		// Allocate buffer to format the message
		char *temp_msg = malloc((msg_bufsz+1)*sizeof(char));

		// Format message
		va_start(args,message);
		vsprintf(temp_msg,message,args);
		va_end(args);

		// Format output
		has_token = replace_token(NULL,temp_out,"%(message)",temp_msg);
		temp_out = (char*)realloc(temp_out,(has_token+1)*sizeof(char));
		bufsz = replace_token(temp_out,temp_out,"%(message)",temp_msg);
		free(temp_msg);
	}

	if(output != NULL) {strcpy(output,temp_out);}

	free(line_num_str);
	free(temp_out);
	free(dt_out);
	return bufsz;
}

size_t vreplace_token(char *output, const char *format_str, const char *token, va_list args)
{
	// The temp output will be iterated over until all token instances
	// are replaced.
	size_t bufsz = 0;
	char *temp_out = (char*)malloc((strlen(format_str) + 1)*sizeof(char));
	strcpy(temp_out,format_str);

	// While tokens are found,get buffer size and reallocate temp_out
	while(1){
		const char *value = va_arg(args, const char*);
		size_t has_token = replace_next_token(NULL,temp_out,token,value);
		if(!has_token) {break;}
		bufsz = has_token;
		temp_out = (char*)realloc(temp_out,(bufsz+1)*sizeof(char));
		replace_next_token(temp_out,temp_out,token,value);
	}

	if(output != NULL) {strcpy(output,temp_out);}

	free(temp_out);
	return bufsz;
}

size_t replace_token(char *output, const char *format_str, const char *token, const char *value)
{
	// The temp output will be iterated over until all token instances
	// are replaced.
	size_t bufsz = 0;
	char *temp_out = (char*)malloc((strlen(format_str) + 1)*sizeof(char));
	strcpy(temp_out,format_str);

	// While tokens are found,get buffer size and reallocate temp_out
	while(1){
		size_t has_token = replace_next_token(NULL,temp_out,token,value);
		if(!has_token) {break;}
		bufsz = has_token;
		temp_out = (char*)realloc(temp_out,(bufsz+1)*sizeof(char));
		replace_next_token(temp_out,temp_out,token,value);
	}

	if(output != NULL) {strcpy(output,temp_out);}

	free(temp_out);
	return bufsz;
}

// Calculate needed buffer size and replace token
//   - start_idx,format_str: copy chars until start of token
//   - value: insert value
//   - temp_fmt: copy chars after token and until '\0'
size_t replace_next_token(char *output, const char *format_str, const char *token, const char *value)
{
	// Copy format string in case it points to output
	char *temp_fmt = (char*)malloc((strlen(format_str)+1)*sizeof(char));
	strcpy(temp_fmt,format_str);

	// Return if token is not found
	const char *start = strstr(temp_fmt, token);
	if(!start) {return 0;}

	// Calculate needed buffer size
	int start_idx = start - temp_fmt;
	int end_idx = start_idx + strlen(token);
	size_t bufsz = snprintf(NULL, 0,"%.*s%s%s", start_idx, temp_fmt, value,
			temp_fmt + end_idx);
	// Replace token
	if(output != NULL) {
		sprintf(output, "%.*s%s%s", start_idx, temp_fmt, value,
				temp_fmt + end_idx);
	}

	free(temp_fmt);
	return bufsz;
}

size_t format_datetime(char *output, const char *format_str)
{
	// Get epoch time
	time_t epoch = time(0);
	// Get current time
	struct tm *current_time = localtime(&epoch);
	// Determine buffer size needed
	// Size can't be less than format string
	size_t bufsz = strlen(format_str) + 1;
	char *buff = (char*)malloc(bufsz*sizeof(char));
	while(!strftime(buff,bufsz,format_str,current_time)) {
		bufsz *= 2;
		buff = (char*)realloc(buff,bufsz*sizeof(char));
	}
	bufsz = strlen(buff);
	// Write datetime formatted string to output
	if(output != NULL) {strcpy(output,buff);}

	free(buff);
	return bufsz;
}
