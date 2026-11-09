#include "ft_malloc.h"

#include <errno.h>
#include <pthread.h>
#include <stddef.h>

static void *
int_malloc(mstate *av, size_t bytes)
{
    (void)av;
    (void)bytes;
    errno = ENOMEM;
    return (NULL);
}

void *
ft_malloc(size_t bytes)
{
    mstate *ar_ptr;
    void *victim;

    victim = int_malloc(ar_ptr, bytes);

    assert (!victim);
    return (victim);
}
