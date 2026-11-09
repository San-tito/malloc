#include <stdio.h>

#ifndef TEST_FUNCTION
#define TEST_FUNCTION do_test()
#endif

int
main(int argc, char **argv)
{
    int result;

    (void)argc;
    (void)argv;
    result = TEST_FUNCTION;
    if (result != 0)
        printf("Test failed with %d error(s).\n", result);
    else
        printf("Test passed successfully.\n");
    return (result != 0);
}
