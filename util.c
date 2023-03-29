#include "util.h"

#include <stdlib.h> // abs

int itoa(int n, char* dst, int len)
{
    // Return early for 0
    if (n == 0 && len >= 2)
    {
        dst[0] = '0';
        dst[1] = '\0';
        return 1;
    }

    // Return early if there's no room for digits
    if (len < 2)
    {
        // Terminate string if there's room
        if (len == 1)
            dst[0] = '\0';

        return 0;
    }

    // Handle negative numbers by remembering the sign
    int negative = 0;
    if (n < 0)
    {
        negative = 1;
        n = abs(n);
    }

    // Divide by 10 and place rest in array until 0
    int i = 0;
    char buffer[64];
    while (n > 0)
    {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
    }
    buffer[i] = '\0';

    // Append sign and reverse array
    int j = 0;
    if (negative)
        dst[j++] = '-';

    for (int k = i - 1; k >= 0 && j < len - 1; k--)
    {
        dst[j++] = buffer[k];
    }
    dst[j] = '\0';

    return i;
}
