#ifndef MALLOC_H
#define MALLOC_H

#include <pthread.h>
#include <stddef.h>
#include <errno.h>

#define MALLOC_ALIGN_MASK      (2 * sizeof(size_t) - 1)
#define MINSIZE                (4 * sizeof(size_t))

#define PREV_INUSE             0x1
#define IS_MMAPPED             0x2
#define SIZE_BITS              (PREV_INUSE | IS_MMAPPED)

#define TINY_MAX               128
#define TINY_ZONE_SIZE         (16 * 1024)

#define SMALL_MAX              1024
#define SMALL_ZONE_SIZE        (128 * 1024)

typedef struct malloc_chunk mchunk;
typedef struct malloc_state mstate;
typedef struct zone         zone_t;

struct malloc_chunk
{
    size_t prev_size;
    size_t size;
    mchunk *fd;
    mchunk *bk;
    mchunk *fd_nextsize;
    mchunk *bk_nextsize;
};

struct zone
{
    zone_t *next;
    size_t  total_size;
    mchunk *top;
};

struct malloc_state
{
    pthread_mutex_t mutex;
    zone_t *tiny_zones;
    zone_t *small_zones;
    mchunk *large_chunks;
};

void *ft_malloc(size_t bytes);
void  show_alloc_mem(void);

void *tiny_malloc(zone_t **zone_list, size_t nb);
void  print_tiny_region(zone_t *list, size_t *total);

void *small_malloc(zone_t **zone_list, size_t nb);
void  print_small_region(zone_t *list, size_t *total);

void *large_malloc(mstate *av, size_t nb);
void  large_debug_print(mchunk *list, size_t *total);

void  print_hexdump(const void *addr, size_t size);

#endif /* !MALLOC_H */
