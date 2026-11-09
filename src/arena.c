#include "arena.h"
#include "malloc.h"

static mstate arena = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .next = &arena,
    .attached_threads = 1
};

mstate *
arena_get(void)
{
    pthread_mutex_lock(&arena.mutex);
    return (&arena);
}
