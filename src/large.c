#include "malloc.h"
#include <sys/mman.h>
#include <stdio.h>

void *
large_malloc(mstate *av, size_t nb)
{
    size_t pagesize = 4096;
    size_t total_size = (nb + 2 * sizeof(size_t) + pagesize - 1) & ~(pagesize - 1);

    char *mm = (char *)mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mm == MAP_FAILED)
    {
        errno = ENOMEM;
        return (NULL);
    }

    mchunk *p = (mchunk *)mm;
    p->prev_size = 0;
    p->size = total_size | IS_MMAPPED;

    p->fd = av->large_chunks;
    av->large_chunks = p;

    return ((void *)((char *)p + 2 * sizeof(size_t)));
}

void
large_debug_print(mchunk *list, size_t *total)
{
    mchunk *p;

    for (p = list; p != NULL; p = p->fd)
    {
        printf("LARGE : %p\n", (void *)p);
        size_t sz = p->size & ~SIZE_BITS;
        void *start = (void *)((char *)p + 2 * sizeof(size_t));
        void *end = (void *)((char *)p + sz);
        size_t user_size = sz - 2 * sizeof(size_t);

        printf("%p - %p : %zu bytes\n", start, end, user_size);
        print_hexdump(start, user_size);
        *total += user_size;
    }
}
