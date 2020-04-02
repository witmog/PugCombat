#pragma once
#include <stdarg.h>
#include <stdio.h>

#define LOG_TRACE(...) LogMessage(LOGLEVEL_TRACE,	\
				  __FILE__,		\
				  __LINE__,		\
				  __VA_ARGS__) 

#define LOG_WARN(...) LogMessage(LOGLEVEL_WARN, \
				 __FILE__,	\
				 __LINE__,	\
				 __VA_ARGS__) 

#define LOG_ERROR(...) LogMessage(LOGLEVEL_ERROR,	\
				  __FILE__,		\
				  __LINE__,		\
				  __VA_ARGS__) 

#define LOG_FATAL(...) LogMessage(LOGLEVEL_FATAL,	\
				  __FILE__,		\
				  __LINE__,		\
				  __VA_ARGS__) 

enum LogLevel
{
    LOGLEVEL_TRACE,
    LOGLEVEL_WARN,
    LOGLEVEL_ERROR,
    LOGLEVEL_FATAL
};

void
LogMessage(enum LogLevel level, char *file, int linenum, char *format, ...);
