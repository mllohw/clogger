/**  clogger header
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

#ifndef CLOGGER_H
#define CLOGGER_H

#include <stddef.h>
#include <stdarg.h>

// All functions return char count without null terminator
size_t format(char *output, const char *format_str, const char *file_name,
		const char *func_name, const int line_num,
		const char *message, ...);

size_t vreplace_token(char *output, const char *format_str, const char *token, va_list args);

size_t replace_token(char *output, const char *format_str, const char *token, const char *value);

size_t replace_next_token(char *output, const char *format_str, const char *token, const char *value);

size_t get_datetime(char *output, const char *format_str);
#endif
