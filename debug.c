#include "debug.h"

#include <stdarg.h> // va_start, va_end
#include <stdio.h> // fopen, fclose, vfprintf, etc
#include <stdlib.h> // getenv

#define FILENAME "debug.out"

void debug(const char *format, ...)
{
    if (getenv("DEBUG") == NULL)
        return;

    va_list args;
    va_start(args, format);

    FILE *fp = fopen(FILENAME, "a");
    if (fp != NULL)
    {
        vfprintf(fp, format, args);
        fclose(fp);
    }
    else
    {
        vfprintf(stderr, format, args);
    }

    va_end(args);
}
