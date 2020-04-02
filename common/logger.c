#include "logger.h"

char *log_label[] = { "TRACE", "WARNING", "ERROR", "FATAL" };

void
LogMessage(enum LogLevel level, char *file,  int linenum, char *format, ...)
{
    va_list args;

    fprintf(stderr,
	    "[%s][%s:%d]: ",
	    log_label[level],
	    file,
	    linenum);
		

    va_start(args, format);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}
