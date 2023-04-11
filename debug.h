#ifndef DEBUG_H
#define DEBUG_H

#ifndef DEBUG
#define DEBUG 0
#endif

#define FILENAME "debug.out"

#define DATETIME_FORMAT "%Y-%m-%dT%H:%M:%S%z"

#define LOG(...) \
    debug(__FILE__, __LINE__, __VA_ARGS__);

void debug(const char *file, int line, const char *format, ...);

#endif
