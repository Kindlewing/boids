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

void *arena_push(arena *arena, u64 size) {
	return arena_push_aligned(arena, size, DEFAULT_ALIGNMENT);
}

void *arena_push_aligned(arena *arena, u64 size, size_t align) {
	uintptr_t ptr = (uintptr_t)arena->base + arena->offset;
	uintptr_t aligned_ptr = align_forward(ptr, align);
	u64 padding = (u64)(aligned_ptr - ptr);

	if(arena->offset + padding + size > arena->capacity) {
		return NULL;
	}
	void *result = (void *)aligned_ptr;
	arena->offset += padding + size;
	return result;
}

void *arena_push_zero(arena *arena, u64 size) {
	void *ptr = arena_push(arena, size);
	if(!ptr)
		return NULL;

	u8 *p = (u8 *)ptr;
	for(u64 i = 0; i < size; i++)
		p[i] = 0;
	return ptr;
}

void arena_clear(arena *arena) {
	arena->offset = 0;
}

void arena_free(arena *arena) {
	munmap(arena, sizeof *arena + arena->capacity);
}
