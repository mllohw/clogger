/**  Header file for clogger formatting
 *
 *   Terminology:
 *   ------------
 *   Format string = "Number: %hd"
 *   Format specifier = "%hd"
 *   Conversion modifier = "h"
 *   Conversion specifier = "d"
 *
 *   Copyright (C) 2025  Mirko Lindroth
 *   SPDX-License-Identifier: MIT
 */

#ifndef FORMATTER_H
#define FORMATTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdarg.h>

// All functions return char count without null terminator
size_t vformat(char *output, const char *format_str, const char *file_name,
		const char *func_name,const int line_num, const char *app,
		const char *msg_type, const char *datetime_fmt,
		const char *message, va_list args);

size_t format(char *output, const char *format_str, const char *file_name,
		const char *func_name,const int line_num, const char *app,
		const char *msg_type, const char *datetime_fmt,
		const char *message, ...);

size_t vreplace_token(char *output, const char *format_str, const char *token,
		va_list args);

size_t replace_token(char *output, const char *format_str, const char *token,
		const char *value);

size_t replace_next_token(char *output, const char *format_str,
		const char *token, const char *value);

size_t format_datetime(char *output, const char *format_str);

#ifdef __cplusplus
}
#endif

#endif
