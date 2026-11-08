#include <stdio.h>
#include <stdlib.h>

#ifndef TEST_FUNCTION
# define TEST_FUNCTION do_test ()
#endif

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    int res = TEST_FUNCTION;
    if (res != 0)
        printf("Test failed with %d errors.\n", res);
    else
        printf("Test passed successfully.\n");
    return res;
}
