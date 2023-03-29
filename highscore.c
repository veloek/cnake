#include "highscore.h"
#include "debug.h"
#include "util.h"

#include <stdio.h> // fopen
#include <stdlib.h> // atoi
#include <errno.h> // errno
#include <string.h> //strerror, strlen

int highscore_init()
{
    FILE* highscore_file = fopen(HIGHSCORE_FILE, "r");

    if (highscore_file == NULL)
    {
        debug("Error opening highscore file %s: %s\n",
                HIGHSCORE_FILE, strerror(errno));
        return 0;
    }

    char buffer[64];
    int n = fread(buffer, sizeof(char), sizeof(buffer), highscore_file);
    if (n <= 0)
    {
        debug("Error reading highscore file %s: %s\n",
                HIGHSCORE_FILE, strerror(errno));
        return 0;
    }

    int highscore = atoi(buffer);

    fclose(highscore_file);

    return highscore;
}

void highscore_update(int highscore)
{
    FILE* highscore_file = fopen(HIGHSCORE_FILE, "w");

    if (highscore_file == NULL)
    {
        debug("Error opening highscore file %s: %s\n",
                HIGHSCORE_FILE, strerror(errno));
        return;
    }

    char buffer[64];
    itoa(highscore, buffer, 64);
    int n = fwrite(buffer, sizeof(char), strlen(buffer), highscore_file);
    if (n <= 0)
    {
        debug("Error writing highscore file %s: %s\n",
                HIGHSCORE_FILE, strerror(errno));
        return;
    }

    fclose(highscore_file);
}
