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
#include <string.h>

#define INT_MAX  2147483647
#define INT_MIN -2147483648

static int n_runs;
static int n_success;

static void test(char* desc, int n, int size, char* expected)
{
    char buf[size];
    int written = util_itoa(n, buf, size);
    int cmp = strcmp(buf, expected);
    if (cmp == 0)
    {
        printf("SUCCESS: %s(%d, %d): '%s'(%d)\n",
                desc, n, size, buf, written);
        n_success++;
    }
    else
    {
        printf("FAILURE: %s(%d, %d): '%s'(%d), expected: %s\n",
                desc, n, size, buf, written, expected);
    }
    n_runs++;
}

int main()
{
    test("iota_should_return_empty_string_given_buffer_size_is_1",
            1, 1, "");
    test("iota_should_return_full_1_given_buffer_size_is_2",
            1, 2, "1");
    test("iota_shoud_return_no_more_than_1_given_buffer_size_is_3",
            1, 3, "1");
    test("iota_shoud_return_only_sign_given_buffer_size_is_2",
            -1, 2, "-");
    test("iota_should_return_full_-1_given_buffer_size_is_3",
            -1, 3, "-1");
    test("iota_should_return_12_out_of_123_given_buffer_size_is_3",
            123, 3, "12");
    test("iota_should_return_full_123_given_buffer_size_is_10",
            123, 10, "123");
    test("iota_should_return_-1_out_of_-123_given_buffer_size_is_3",
            -123, 3, "-1");
    test("iota_should_return_full_-123_given_buffer_size_is_10",
            -123, 10, "-123");
    test("iota_should_return_INT_MAX_given_INT_MAX",
            INT_MAX, 16, "2147483647");
    test("iota_should_return_INT_MIN+1_given_INT_MIN+1",
            INT_MIN+1, 16, "-2147483647");
    test("iota_should_return_-_given_INT_MIN_due_to_overflow",
            INT_MIN, 16, "-");

    printf("-TOTAL-: %d/%d\n", n_success, n_runs);
}
