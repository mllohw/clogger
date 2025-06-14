/**  Example of formatted print
 *
 *   Copyright (C) 2025  Mirko Lindroth
 *   SPDX-License-Identifier: MIT
 */
#include <clogger/formatter.h>
#include <stdlib.h>
#include <stdio.h>

void my_print(const char *message,...)
{
	va_list args;

	// Format string to use
	const char fmt_str[] = "[app] %(datetime) %(file)[%(func)](%(line)): \n"
		"[\033[32m INFO \033[0m] %(message)\n";

	// Calculate the needed size
	va_start(args,message);
	size_t bufsz = format(NULL,fmt_str,__FILE__,__func__,__LINE__,message,args);
	va_end(args);

	// Store the processed format string
	char *msg = malloc((bufsz+1)*sizeof(char));
	va_start(args,message);
	vformat(msg,fmt_str,__FILE__,__func__,__LINE__,message,args);
	va_end(args);

	// Print the message to stdout
	fprintf(stdout,"%s",msg);

	free(msg);
}

int main()
{
	my_print("Hello World!");
	int x = 1;
	printf("x is %d\n",x);
	my_print("Value of x is %d\n",x);
	return EXIT_SUCCESS;
}
