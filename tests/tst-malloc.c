#include "malloc.h"

static int errors = 0;

static void merror(const char *msg)
{
    ++errors;
    printf("Error: %s\n", msg);
}

static int do_test(void)
{
    void *p, *q;
    int save;

    errno = 0;

    p = ft_malloc((size_t)-1);
    save = errno;

    if (p != NULL)
        merror("ft_malloc ((size_t)-1) succeeded.");
    if (p == NULL && save != ENOMEM)
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

    free(p);

    return errors != 0;
}

#define TEST_FUNCTION do_test()
#include "test-skeleton.c"
