#ifndef ARENA_H
#define ARENA_H

#include "typedefs.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct {
    u8 *base;
    u64 capacity;
    u64 offset;
} arena;

arena *arena_create(u64 capacity);
void arena_clear(arena *arena);
void arena_free(arena *arena);

void *arena_alloc(arena *arena, u64 size);
void *arena_alloc_aligned(arena *arena, u64 size, size_t align);

#endif // ARENA_H
