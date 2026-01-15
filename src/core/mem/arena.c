#include "arena.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

static inline bool is_power_of_two(uintptr_t x) {
    return (x & (x - 1)) == 0;
}

static uintptr_t align_forward(uintptr_t ptr, size_t align) {
    uintptr_t p, a, modulo;

    assert(is_power_of_two(align));

    p = ptr;
    a = (uintptr_t)align;
    modulo = p & (a - 1);

    if(modulo != 0) {
        p += a - modulo;
    }
    return p;
}

arena *arena_create(u64 capacity) {
    arena *arena = malloc(sizeof *arena + capacity);
    arena->memory = (u8 *)arena + sizeof *arena;
    arena->capacity = capacity;
    arena->offset = 0;
    return arena;
}

void *arena_alloc(arena *arena, u64 size) {
    if(arena->offset + size >= arena->capacity) {
        return NULL;
    }
    void *ptr = &arena->memory[arena->offset];
    arena->offset += size;
    return ptr;
}

void *arena_alloc_aligned(arena *arena, u64 size, size_t align) {
    uintptr_t aligned_ptr = (uintptr_t)arena->memory + (uintptr_t)arena->offset;
    uintptr_t aligned_offset = align_forward(aligned_ptr, align);
    u64 padding = (u64)(aligned_offset - aligned_ptr);
    if(arena->offset + padding + size > arena->capacity) {
        return NULL;
    }
    void *ptr = (void *)aligned_ptr;
    arena->offset += padding + size;
    return ptr;
}

void arena_clear(arena *arena) {
    arena->offset = 0;
}

void arena_free(arena *arena) {
    free(arena);
}
