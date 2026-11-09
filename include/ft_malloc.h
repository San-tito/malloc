#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <pthread.h>
#include <stddef.h>

#define NBINS 128

#define BINMAPSIZE (NBINS / (sizeof(unsigned int) * 8))

#define MAX_FAST_SIZE (80 * sizeof(size_t) / 4)

#define NFASTBINS (MAX_FAST_SIZE / (2 * sizeof(size_t)))

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

void *ft_malloc(size_t size_bytes);

#endif /* !FT_MALLOC_H */
