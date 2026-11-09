#include "malloc.h"
#include "arena.h"
#include <sys/mman.h>
#include <unistd.h>

static size_t
request2size(size_t req)
{
    if (req + sizeof(size_t) + MALLOC_ALIGN_MASK < MINSIZE)
        return (MINSIZE);
    return ((req + sizeof(size_t) + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK);
}

static void *
sysmalloc(size_t nb, mstate *av)
{
    mchunk *p;
    size_t size;
    size_t pagesize = 4096;

    if (nb >= DEFAULT_MMAP_THRESHOLD)
    {
        size = (nb + sizeof(size_t) + pagesize - 1) & ~(pagesize - 1);
        char *mm = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mm != MAP_FAILED)
        {
            p = (mchunk *)mm;
            p->prev_size = 0;
            p->size = size | IS_MMAPPED;
            return ((void *)((char *)p + 2 * sizeof(size_t)));
        }
    }

    size = MMAP_AS_MORECORE_SIZE;
    if (size < nb + MINSIZE)
        size = (nb + MINSIZE + pagesize - 1) & ~(pagesize - 1);

    char *mbrk = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mbrk == MAP_FAILED)
    {
        errno = ENOMEM;
        return (NULL);
    }

    av->top = (mchunk *)mbrk;
    av->top->prev_size = 0;
    av->top->size = size | PREV_INUSE;
    av->system_mem += size;

    p = av->top;
    size = p->size & ~SIZE_BITS;
    if (size >= nb + MINSIZE)
    {
        size_t remainder_size = size - nb;
        mchunk *remainder = (mchunk *)((char *)p + nb);

        av->top = remainder;
        p->size = nb | PREV_INUSE;
        remainder->size = remainder_size | PREV_INUSE;

        return ((void *)((char *)p + 2 * sizeof(size_t)));
    }

    errno = ENOMEM;
    return (NULL);
}

static void *
internal_malloc(mstate *av, size_t bytes)
{
    size_t nb;
    mchunk *victim;
    size_t size;
    mchunk *remainder;
    size_t remainder_size;

    nb = request2size(bytes);
    if (nb < bytes || nb >= (size_t)(-2 * MINSIZE))
    {
        errno = ENOMEM;
        return (NULL);
    }

    victim = av->top;
    if (victim != NULL)
    {
        size = victim->size & ~SIZE_BITS;
        if (size >= nb + MINSIZE)
        {
            remainder_size = size - nb;
            remainder = (mchunk *)((char *)victim + nb);

            av->top = remainder;
            victim->size = nb | PREV_INUSE;
            remainder->size = remainder_size | PREV_INUSE;

            return ((void *)((char *)victim + 2 * sizeof(size_t)));
        }
    }

    return (sysmalloc(nb, av));
}

void *
ft_malloc(size_t bytes)
{
    mstate *ar_ptr = NULL;
    void   *victim = NULL;

    ar_ptr = arena_get();
    victim = internal_malloc(ar_ptr, bytes);

    if (ar_ptr != NULL)
        pthread_mutex_unlock(&ar_ptr->mutex);

    return (victim);
}
