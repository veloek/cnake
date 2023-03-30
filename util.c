#include "util.h"

int util_itoa(int num, char* dst, int size)
{
    // Return early for 0
    if (num == 0 && size >= 2)
    {
        dst[0] = '0';
        dst[1] = '\0';
        return 1;
    }

    // Return early if there's no room in dst
    if (size < 2)
    {
        // Terminate string if there's room
        if (size == 1)
            dst[0] = '\0';

        return 0;
    }

    // Handle negative numbers by remembering the sign
    int negative = 0;
    if (num < 0)
    {
        negative = 1;
        num = -num;
    }

    // Divide by 10 and place rest in buffer until 0
    int i = 0;
    char buf[11]; // Room to hold the largest integer incl. sign
    while (num > 0)
    {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Append sign if negative
    if (negative)
        buf[i++] = '-';

    // Reverse array into dst
    int j = 0;
    while (j < i && j < size - 1)
    {
        char c = buf[i - 1 - j];
        dst[j++] = c;
    }
    dst[j] = '\0';

    return j;
}
