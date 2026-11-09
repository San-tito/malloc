#include "ft_malloc.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int errors = 0;

static void
merror(const char *msg)
{
    ++errors;
    printf("Error: %s\n", msg);
}

static int
do_test(void)
{
    void *p;
    void *q;
    int errno_saved;

    errno = 0;
    p = ft_malloc((size_t)-1);
    errno_saved = errno;
    if (p != NULL)
        merror("ft_malloc ((size_t)-1) succeeded.");
    if (p == NULL && errno_saved != ENOMEM)
        merror("errno is not set to ENOMEM on overflow.");

    p = ft_malloc(10);
    if (p == NULL)
        merror("ft_malloc (10) failed.");

    p = ft_malloc(0);
    if (p == NULL)
        merror("ft_malloc (0) failed.");

    p = ft_malloc(513 * 1024);
    if (p == NULL)
        merror("ft_malloc (513K) failed.");

    q = ft_malloc((size_t)-512 * 1024);
    if (q != NULL)
        merror("ft_malloc (-512K) succeeded.");

    (void)p;
    (void)q;
    return (errors);
}

#define TEST_FUNCTION do_test()
#include "test_skeleton.c"
