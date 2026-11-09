#include "malloc.h"
#include "arena.h"
#include <stdio.h>

static size_t
request2size(size_t req)
{
    if (req + sizeof(size_t) + MALLOC_ALIGN_MASK < MINSIZE)
        return (MINSIZE);
    return ((req + sizeof(size_t) + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK);
}

static void *
int_malloc(mstate *av, size_t bytes)
{
    size_t nb;

    nb = request2size(bytes);
    if (nb < bytes || nb >= (size_t)(-2 * MINSIZE))
    {
        errno = ENOMEM;
        return (NULL);
    }

    if (bytes <= TINY_MAX)
        return (tiny_malloc(&av->tiny_zones, nb));
    if (bytes <= SMALL_MAX)
        return (small_malloc(&av->small_zones, nb));
    return (large_malloc(av, nb));
}

void *
ft_malloc(size_t bytes)
{
    mstate *ar_ptr = NULL;
    void   *victim = NULL;

    ar_ptr = arena_get();
    victim = int_malloc(ar_ptr, bytes);

    if (ar_ptr != NULL)
        pthread_mutex_unlock(&ar_ptr->mutex);

    return (victim);
}

void
print_hexdump(const void *addr, size_t size)
{
    const unsigned char *p = (const unsigned char *)addr;
    size_t i, j;

    for (i = 0; i < size; i += 16)
    {
        printf("  %p: ", (const void *)(p + i));
        for (j = 0; j < 16; j++)
        {
            if (i + j < size)
                printf("%02x ", p[i + j]);
            else
                printf("   ");
            if (j == 7)
                printf(" ");
        }
        printf(" |");
        for (j = 0; j < 16 && (i + j) < size; j++)
        {
            unsigned char c = p[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        printf("|\n");
    }
}

void
show_alloc_mem(void)
{
    mstate *av = arena_get();
    size_t  total = 0;

    print_tiny_region(av->tiny_zones, &total);
    print_small_region(av->small_zones, &total);
    large_debug_print(av->large_chunks, &total);

    printf("Total : %zu bytes\n", total);
    pthread_mutex_unlock(&av->mutex);
}
