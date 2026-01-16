#include "arena.h"
#include <assert.h>

static inline bool is_power_of_two(uintptr_t x) {
    return (x & (x - 1)) == 0;
}

static uintptr_t align_forward(uintptr_t ptr, size_t align) {
    uintptr_t p = ptr;
    uintptr_t a;
    uintptr_t modulo;
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
    arena *arena = mmap(NULL, sizeof *arena + capacity, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(arena == MAP_FAILED)
        return NULL;
    arena->base = (u8 *)arena + sizeof *arena;
    arena->capacity = capacity;
    arena->offset = 0;
    return arena;
}

void *arena_alloc(arena *arena, u64 size) {
    return arena_alloc_aligned(arena, size, DEFAULT_ALIGNMENT);
}

void *arena_alloc_aligned(arena *arena, u64 size, size_t align) {
    uintptr_t ptr = (uintptr_t)arena->base + (uintptr_t)arena->offset;
    uintptr_t aligned_offset = align_forward(ptr, align);
    u64 padding = (u64)(aligned_offset - ptr);
    if(arena->offset + padding + size > arena->capacity) {
        return NULL;
    }
    void *ptr_aligned = (void *)aligned_offset;
    arena->offset += padding + size;
    return ptr_aligned;
}

void arena_clear(arena *arena) {
    arena->offset = 0;
}

void arena_free(arena *arena) {
    munmap(arena, sizeof *arena + arena->capacity);
}
