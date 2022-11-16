#include "log.h"

#include <stdarg.h> // va_start, va_end
#include <stdio.h> // fopen, fclose, vfprintf, etc
#include <stdlib.h> // getenv
#include <string.h> // strlen
#include <time.h> // time, strftime

#define LOG_FILENAME "cnake.log"

static void log(const char *fmt, const char *prefix, va_list args)
{
    if (getenv("LOGGING") == NULL)
        return;

    char timestamp[23];
    time_t now = time(NULL);
    strftime(timestamp, 23, "%Y-%m-%d %H:%M:%S | ", localtime(&now));

    char format[strlen(timestamp) + strlen(prefix) + strlen(fmt) + 1];
    strcpy(format, timestamp);
    strcpy(format + strlen(timestamp), prefix);
    strcpy(format + strlen(timestamp) + strlen(prefix), fmt);

    FILE *fp = fopen(LOG_FILENAME, "a");
    if (fp != NULL)
    {
        vfprintf(fp, format, args);
        fclose(fp);
    }
    else
    {
        vfprintf(stderr, fmt, args);
    }
}

void log_debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    log(fmt, "DEBUG | ", args);

    va_end(args);
}

void log_warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    log(fmt, "WARN | ", args);

    va_end(args);
}

void log_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    log(fmt, "ERROR | ", args);

    va_end(args);
}
