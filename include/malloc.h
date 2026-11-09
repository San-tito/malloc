#ifndef MALLOC_H
#define MALLOC_H

#include <pthread.h>
#include <stddef.h>
#include <errno.h>

#define NBINS                  128
#define BINMAPSIZE             (NBINS / (sizeof(unsigned int) * 8))
#define NFASTBINS              10

#define MALLOC_ALIGN_MASK      (2 * sizeof(size_t) - 1)
#define MINSIZE                (4 * sizeof(size_t))

#define DEFAULT_MMAP_THRESHOLD (128 * 1024)
#define MMAP_AS_MORECORE_SIZE  (1024 * 1024)

#define PREV_INUSE             0x1
#define IS_MMAPPED             0x2
#define SIZE_BITS              (PREV_INUSE | IS_MMAPPED)

typedef struct malloc_chunk mchunk;
typedef struct malloc_state mstate;

struct malloc_chunk
{
    size_t prev_size;
    size_t size;
    mchunk *fd;
    mchunk *bk;
    mchunk *fd_nextsize;
    mchunk *bk_nextsize;
};

struct malloc_state
{
    pthread_mutex_t mutex;
    int flags;
    int have_fastchunks;
    mchunk *fastbins[NFASTBINS];
    mchunk *top;
    mchunk *last_remainder;
    mchunk *bins[NBINS * 2 - 2];
    unsigned int binmap[BINMAPSIZE];
    mstate *next;
    mstate *next_free;
    size_t attached_threads;
    size_t system_mem;
    size_t max_system_mem;
};

void *ft_malloc(size_t bytes);

#endif /* !MALLOC_H */
