/**  Tests for clogger
 *
 *   Copyright (C) 2025  Mirko Lindroth
 *   SPDX-License-Identifier: MIT
 */
#ifndef CLOGGER_TEST
#define CLOGGER_TEST

#include <clogger/formatter.h>
#include <clogger/clogger.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void tc_datetime();
void tc_replace_next_token();
void tc_replace_token();
void tc_vreplace_token(const char *ref_out,...);
void tc_format();
void tc_vformat(const char format_str[],const char ref_out[],
		const char msg[],...);
void tc_msg_with_verb_ctrl(struct clogger_conf conf);
void tc_verbosity_ctrl(struct clogger_conf conf);
void tc_reporting_macros(struct clogger_conf conf);

int main()
{
	// Test formatting
	tc_datetime();
	tc_replace_next_token();
	tc_replace_token();
	tc_vreplace_token("some/long/path/here/abc.c "
			"another/longer/path/to/test/this/def.c ghi.c",
			"some/long/path/here/abc.c",
			"another/longer/path/to/test/this/def.c", "ghi.c");
	tc_format();
	tc_vformat("%(message)","Hello World!\n","Hello %s\n","World!");

	// Test logging
	struct clogger_conf conf = {
		.verbosity=AVG,
		.app_name="clogger",
		.log_file="clogger.log",
		.datetime_fmt="%Y-%m-%d %Z %H:%M:%S",
		.fmt={
			.console_format="[%(app)] %(datetime) "
				"\033[32m%(msg_type)\033[0m %(file): "
				"%(func)(%(line)) \n%(message)",
			.log_file_format="[%(app)] %(datetime) "
				"%(msg_type) %(file): "
				"%(func)(%(line)) \n%(message)"
		}
	};
	remove(conf.log_file);

	tc_msg_with_verb_ctrl(conf);
	tc_verbosity_ctrl(conf);
	tc_reporting_macros(conf);

	return EXIT_SUCCESS;
}

void tc_reporting_macros(struct clogger_conf conf)
{
	info(conf,LOW,"Reporting macro with %s verbosity\n",MACRO_NAME(LOW));
}

void tc_verbosity_ctrl(struct clogger_conf conf)
{
	// This should not print
	conf.verbosity = NONE;
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,LOW,"INFO",
			"Verbosity is %s and message verbosity %s\n",
			MACRO_NAME(NONE),MACRO_NAME(LOW));
	// This should print
	conf.verbosity = HIGH;
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,LOW,"INFO",
			"Verbosity is %s and message verbosity %s\n",
			MACRO_NAME(HIGH),MACRO_NAME(LOW));
}

void tc_msg_with_verb_ctrl(struct clogger_conf conf)
{
	clog_msg_with_verb_ctrl(conf,__FILE__,__func__,__LINE__,LOW,"INFO",
			"Hello %s","World!\n");
}

void tc_vformat(const char format_str[],const char ref_out[],
		const char msg[],...)
{
	va_list args,args2;

	// Test printing variadic message with va_list
	va_start(args,msg);
	va_copy(args2,args);
	size_t bufsz = vformat(NULL,format_str,"test.c","myfunc",42,"clogger",
			"INFO","%Y-%m-%d %Z %H:%M:%S",msg,args);
	va_end(args);
	// Test buffer size return value matches ref_out size
	if(bufsz != strlen(ref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	// Test formatting of output
	va_start(args,msg);
	char *output = malloc((bufsz+1)*sizeof(char));
	vformat(output,format_str,"test.c","myfunc",42,"clogger",
			"INFO","%Y-%m-%d %Z %H:%M:%S",msg,args);
	if(strcmp(output,ref_out)) {exit(1);}
	printf("[Success] %s: %s",__func__,output);
	va_end(args);
}

void tc_format()
{
	// Test different format specifiers
	char format_str[] = "[%(app)] %(msg_type) %(file): %(func)(%(line))";
	char ref_out[] = "[clogger] INFO test.c: myfunc(42)";
	size_t bufsz = format(NULL,format_str,"test.c","myfunc",42,"clogger",
			"INFO","%Y-%m-%d %Z %H:%M:%S",NULL);
	// Test buffer size return value matches ref_out size
	if(bufsz != strlen(ref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	// Test formatting of output
	char *output = malloc((bufsz+1)*sizeof(char));
	format(output,format_str,"test.c","myfunc",42,"clogger",
			"INFO","%Y-%m-%d %Z %H:%M:%S",NULL);
	if(strcmp(output,ref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,output);

	// Test multiple same format specifiers
	char mformat_str[] = "%(file) %(file)";
	char mref_out[] = "/some/path/test.c /some/path/test.c";
	bufsz = format(NULL,mformat_str,"/some/path/test.c",__func__,__LINE__,
			"clogger","INFO","%Y-%m-%d %Z %H:%M:%S",NULL);
	// Test buffer size return value matches ref_out
	if(bufsz != strlen(mref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	// Test formatting of output
	char *moutput = malloc((bufsz+1)*sizeof(char));
	format(moutput,mformat_str,"/some/path/test.c",__func__,__LINE__,
			"clogger","INFO","%Y-%m-%d %Z %H:%M:%S",NULL);
	if(strcmp(moutput,mref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,moutput);

	// Test with variadic format specifier
	char vformat_str[] = "[%(app)] %(msg_type) %(file)[%(func)](%(line)): %(message)";
	char vref_out[] = "[clogger] INFO test.c[myfunc](42): Hello World!\n";
	bufsz = format(NULL,vformat_str,"test.c","myfunc",42,
			"clogger","INFO","%Y-%m-%d %Z %H:%M:%S",
			"Hello %s\n","World!");
	// Test buffer size return value matches ref_out
	if(bufsz != strlen(vref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	// Test formatting of output
	char *voutput = malloc((bufsz+1)*sizeof(char));
	format(voutput,vformat_str,"test.c","myfunc",42,
			"clogger","INFO","%Y-%m-%d %Z %H:%M:%S",
			"Hello %s\n","World!");
	if(strcmp(voutput,vref_out)) {exit(1);}
	printf("[Success] %s: %s",__func__,voutput);

	free(output);
	free(moutput);
	free(voutput);
}

void tc_vreplace_token(const char *ref_out,...)
{
	va_list args;

	size_t bufsz;
	char format_str[] = "%(file) %(file) %(file)";
	char output[1024];

	// Test return value to calculate needed size
	va_start(args,ref_out);
	bufsz = vreplace_token(NULL,format_str,"%(file)",args);
	if(bufsz != strlen(ref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	va_end(args);

	// Test function correctness with multiple different tokens
	va_start(args,ref_out);
	vreplace_token(output, format_str, "%(file)", args);
	printf("[Success] %s: %s\n",__func__,output);
	if(strcmp(output,ref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,output);
	va_end(args);

}

void tc_replace_token()
{
	size_t bufsz;
	char ref_out[] = "some/path/test.c some/path/test.c some/path/test.c";
	char format_str[] = "%(file) %(file) %(file)";
	char output[1024];

	// Test return value to calculate needed size
	bufsz = replace_token(NULL,format_str,"%(file)", "some/path/test.c");
	if(bufsz != strlen(ref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);

	// Test function correctness with multiple different tokens
	replace_token(output, format_str, "%(file)", "some/path/test.c");
	if(strcmp(output,ref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,output);
}

void tc_datetime()
{
	char datetime_fmt[] = "%Y-%m-%d %Z %H:%M:%S";
	size_t bufsz = format_datetime(NULL,datetime_fmt);
	// Testing return value to calculate needed size
	char *datetime_output = (char*)malloc((bufsz+1)*sizeof(char));
	format_datetime(datetime_output,datetime_fmt);
	printf("[Success] %s: %s\n",__func__,datetime_output);
	free(datetime_output);
}

void tc_replace_next_token()
{
	size_t bufsz;
	char ref_out[] = "2025 some/path/test.c 42 hello";
	char format_str[] = "%(datetime) %(file) %(line) %(message)";
	char output[1024];

	// Test return value to calculate needed size
	bufsz = replace_next_token(NULL,"%(file)","%(file)","test.c") + 1;
	if(bufsz != strlen("test.c")+1) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);

	// Test return value when there is no token
	bufsz = replace_next_token(NULL,"%(line)","%(file)","test.c");
	if(bufsz) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);

	// Test function correctness with multiple different tokens
	replace_next_token(output, format_str, "%(datetime)", "2025");
	replace_next_token(output, output, "%(file)", "some/path/test.c");
	replace_next_token(output, output, "%(line)", "42");
	replace_next_token(output, output, "%(message)", "hello");
	if(strcmp(output,ref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,output);
}

#endif
