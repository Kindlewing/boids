#ifndef ARENA_H
#define ARENA_H

#include "typedefs.h"
#include <stddef.h>

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct {
    u8 *memory;
    u64 capacity;
    u64 offset;
} arena;

arena *arena_create(u64 capacity);
void arena_clear(arena *arena);
void arena_free(arena *arena);

void *arena_alloc(arena *arena, u64 size);
void *arena_alloc_aligned(arena *arena, u64 size, size_t align);

static inline bool is_power_of_two(uintptr_t x);
static uintptr_t align_forward(uintptr_t ptr, size_t align);

#endif // ARENA_H
