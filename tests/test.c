/**  Tests for clogger
 *
 *   Copyright (C) 2025  Mirko Lindroth
 *   SPDX-License-Identifier: MIT
 */
#ifndef CLOGGER_TEST
#define CLOGGER_TEST

#include <clogger/formatter.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void tc_datetime();
void tc_replace_next_token();
void tc_replace_token();
void tc_vreplace_token(const char *ref_out,...);
void tc_format();

int main()
{
	tc_datetime();
	tc_replace_next_token();
	tc_replace_token();
	tc_vreplace_token("some/long/path/here/abc.c "
			"another/longer/path/to/test/this/def.c ghi.c",
			"some/long/path/here/abc.c",
			"another/longer/path/to/test/this/def.c", "ghi.c");
	tc_format();
	return EXIT_SUCCESS;
}

void tc_format()
{
	// Test simple formatting
	char format_str[] = "[app] %(file): %(func)(%(line))";
	char ref_out[] = "[app] test.c: myfunc(42)";
	size_t bufsz = format(NULL,format_str,"test.c","myfunc",42,NULL);
	// Test buffer size return value matches ref_out
	if(bufsz != strlen(ref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	// Test formatting of output
	char *output = malloc((bufsz+1)*sizeof(char));
	format(output,format_str,"test.c","myfunc",42,NULL);
	if(strcmp(output,ref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,output);

	// Test multiple same format specifiers
	char mformat_str[] = "%(file) %(file) %(file)";
	char mref_out[] = "/some/path/test.c ""/some/path/test.c "
		"/some/path/test.c";
	bufsz = format(NULL,mformat_str,"/some/path/test.c",__func__,__LINE__,NULL);
	// Test buffer size return value matches ref_out
	if(bufsz != strlen(mref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	// Test formatting of output
	char *moutput = malloc((bufsz+1)*sizeof(char));
	format(moutput,mformat_str,"/some/path/test.c",__func__,__LINE__,NULL);
	if(strcmp(moutput,mref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,moutput);

	// Test with variadic format specifier
	char vformat_str[] = "[app] %(file)[%(func)](%(line)): %(message)";
	char vref_out[] = "[app] test.c[myfunc](42): Hello World!\n";
	bufsz = format(NULL,vformat_str,
			"test.c","myfunc",42,"Hello %s\n",
			"World!");
	// Test buffer size return value matches ref_out
	if(bufsz != strlen(vref_out)) {exit(1);}
	printf("[Success] %s: Buffer size = %td\n",__func__,bufsz);
	// Test formatting of output
	char *voutput = malloc((bufsz+1)*sizeof(char));
	format(voutput,vformat_str,
			"test.c","myfunc",42,"Hello %s\n",
			"World!");
	if(strcmp(voutput,vref_out)) {exit(1);}
	printf("[Success] %s: %s\n",__func__,voutput);

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
	size_t bufsz = get_datetime(NULL,datetime_fmt) + 1;
	// Testing return value when output is not large enough
	char *datetime_output = (char*)malloc((bufsz-1)*sizeof(char));
	size_t ret = get_datetime(datetime_output,datetime_fmt);
	if(!ret) {
		free(datetime_output);
		exit(1);
	}
	// Testing return value to calculate needed size
	datetime_output = (char*)realloc(datetime_output, bufsz*sizeof(char));
	if(!get_datetime(datetime_output,datetime_fmt)) {
		free(datetime_output);
		exit(1);
	}
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
