#if 0
# Turn file into self-compiling executable
SRC=$(echo $0 | sed 's/_test//')
EXE=${0%.c}
gcc $0 $SRC -o $EXE && ./$EXE
rm -f $EXE
exit
#endif

#include "util.h"

#include <stdio.h>
#include <strings.h>

#define INT_MAX  2147483647
#define INT_MIN -2147483648

void test(int n, int size, char* expected)
{
    char buf[size];
    int written = util_itoa(n, buf, size);
    int cmp = strcmp(buf, expected);
    if (cmp == 0)
    {
        printf("SUCC: test(%d, %d): '%s' (%d)\n", n, size, buf, written);
    }
    else
    {
        printf("FAIL: test(%d, %d): '%s' (%d), expected: %s\n",
                n, size, buf, written, expected);
    }
}

int main()
{
    test(1, 0, "");
    test(1, 1, "");
    test(1, 2, "1");
    test(1, 3, "1");
    test(-1, 2, "-");
    test(-1, 3, "-1");
    test(123, 3, "12");
    test(123, 10, "123");
    test(-123, 3, "-1");
    test(-123, 10, "-123");
    test(INT_MAX, 16, "2147483647");
    test(INT_MIN+1, 16, "-2147483647");
    test(INT_MIN, 16, "-"); // Breaks due to overflow
}
