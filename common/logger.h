#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>


typedef void(*fatal_callback_fn)(void);

enum LogLevel
{
	LOGLEVEL_DEBUG = 0,
	LOGLEVEL_TRACE,
	LOGLEVEL_WARN,
	LOGLEVEL_ERROR,
	LOGLEVEL_FATAL
};

#define log_debug(...) log_write_message(LOGLEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_trace(...) log_write_message(LOGLEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) log_write_message(LOGLEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_write_message(LOGLEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_write_message(LOGLEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#define log_assert(condition) \
	(void)((condition) || (log_fatal("Assert (%s) failed", #condition),0))

void
log_write_message(enum LogLevel level, char *filename, int line_number, char *fmt, ...);

void
log_set_file(FILE *file);

void
log_write_to_file_only(bool enable);

void
log_set_minimum_level(enum LogLevel level);

void
log_set_fatal_callback(fatal_callback_fn function);
