#include "logger.h"

#include "language_sugar.h"

#include <stdlib.h>

char *log_level_label[] = {
	"DEBUG",
	"TRACE",
	"WARN",
	"ERROR",
	"FATAL"
};

struct LoggerInfo
{
	FILE *file;
	bool only_write_to_file;
	enum LogLevel minimum_level;
	fatal_callback_fn fatal_callback;
};
global struct LoggerInfo log_info = {
	.file = NULL,
	.only_write_to_file = false,
	.minimum_level = LOGLEVEL_DEBUG,
	.fatal_callback = NULL
};

void
log_write_message(enum LogLevel level, char *file_name, int line_number, char *fmt, ...)
{

	if (level < log_info.minimum_level)
	{
		return;
	}

	if (!log_info.only_write_to_file)
	{
		va_list args;
		fprintf(stderr, "%s  %s:%d:  ", log_level_label[level], file_name, line_number);
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		fprintf(stderr, "\n");
		va_end(args);
		fflush(stderr);
	}

	if (log_info.file)
	{
		va_list args;
		fprintf(log_info.file, "%s  %s:%d:  ", log_level_label[level], file_name, line_number);
		va_start(args, fmt);
		vfprintf(log_info.file, fmt, args);
		fprintf(log_info.file, "\n");
		va_end(args);
		fflush(log_info.file);
	}

	if (level >= LOGLEVEL_FATAL)
	{
		if (log_info.fatal_callback != NULL)
		{
			log_info.fatal_callback();
		}
		else
		{
			abort();
		}
	}
}

void
log_set_file(FILE *file)
{
	log_info.file = file;
}

void
log_write_to_file_only(bool enable)
{
	log_info.only_write_to_file = enable;
}

void
log_set_minimum_level(enum LogLevel level)
{
	log_info.minimum_level = level;
}

void
log_set_fatal_callback(fatal_callback_fn function)
{
	log_info.fatal_callback = function;
}
