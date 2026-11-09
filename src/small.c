#include "malloc.h"
#include <sys/mman.h>
#include <stdio.h>

void *
small_malloc(zone_t **zone_list, size_t nb)
{
    zone_t *z;
    mchunk *victim;
    size_t size;
    size_t remainder_size;
    mchunk *remainder;

    for (z = *zone_list; z != NULL; z = z->next)
    {
        victim = z->top;
        if (victim != NULL)
        {
            size = victim->size & ~SIZE_BITS;
            if (size >= nb + MINSIZE)
            {
                remainder_size = size - nb;
                remainder = (mchunk *)((char *)victim + nb);

                z->top = remainder;
                victim->size = nb | PREV_INUSE;
                remainder->size = remainder_size | PREV_INUSE;

                return ((void *)((char *)victim + 2 * sizeof(size_t)));
            }
        }
    }

    char *space = (char *)mmap(NULL, SMALL_ZONE_SIZE, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (space == MAP_FAILED)
    {
        errno = ENOMEM;
        return (NULL);
    }

    z = (zone_t *)space;
    z->total_size = SMALL_ZONE_SIZE;
    z->next = *zone_list;
    *zone_list = z;

    size_t offset = (sizeof(zone_t) + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK;
    victim = (mchunk *)(space + offset);
    size = SMALL_ZONE_SIZE - offset;

    remainder_size = size - nb;
    remainder = (mchunk *)((char *)victim + nb);

    z->top = remainder;
    victim->size = nb | PREV_INUSE;
    remainder->size = remainder_size | PREV_INUSE;

    return ((void *)((char *)victim + 2 * sizeof(size_t)));
}

void
print_small_region(zone_t *list, size_t *total)
{
    zone_t *z;
    mchunk *p;
    size_t offset = (sizeof(zone_t) + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK;

    for (z = list; z != NULL; z = z->next)
    {
        printf("SMALL : %p\n", (void *)z);
        p = (mchunk *)((char *)z + offset);
        while (p != z->top && (char *)p < ((char *)z + z->total_size))
        {
            size_t sz = p->size & ~SIZE_BITS;
            if (sz == 0)
                break;
            void *start = (void *)((char *)p + 2 * sizeof(size_t));
            void *end = (void *)((char *)p + sz);
            size_t user_size = sz - 2 * sizeof(size_t);

            printf("%p - %p : %zu bytes\n", start, end, user_size);
            print_hexdump(start, user_size);
            *total += user_size;

            p = (mchunk *)((char *)p + sz);
        }
    }
}
