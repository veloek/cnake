#include "debug.h"

#include <stdarg.h> // va_start, va_end
#include <stdio.h> // fopen, fclose, vfprintf, etc
#include <time.h> // time, localtime, strftime

static void get_time(char *out, int size)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(out, size, DATETIME_FORMAT, tm);
}

void debug(const char *file, int line, const char *format, ...)
{
    if (!DEBUG)
        return;

    va_list args;
    va_start(args, format);

    FILE *fp = fopen(FILENAME, "a");
    if (fp != NULL)
    {
        char timestr[64];
        get_time(timestr, sizeof(timestr));
        fprintf(fp, "%s %s:%d ", timestr, file, line);
        vfprintf(fp, format, args);
        fclose(fp);
    }
    else
    {
        vfprintf(stderr, format, args);
    }

    va_end(args);
}
