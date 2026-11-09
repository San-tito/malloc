#include "arena.h"
#include "malloc.h"

static mstate arena = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .tiny_zones = NULL,
    .small_zones = NULL,
    .large_chunks = NULL
};

mstate *
arena_get(void)
{
    pthread_mutex_lock(&arena.mutex);
    return (&arena);
}
