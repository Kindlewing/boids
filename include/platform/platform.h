#ifndef PLATFORM_H
#define PLATFORM_H
#include "base/arena.h"
#include "base/string8.h"
#include <third_party/glad/gl.h>

#define PLATFORM_MAX_KEYS 256
#define PLATFORM_MAX_MOUSE_BUTTONS 8

typedef struct platform_window platform_window;

typedef struct {
	b8 keys_down[PLATFORM_MAX_KEYS];
	b8 keys_pressed[PLATFORM_MAX_KEYS];
	b8 keys_released[PLATFORM_MAX_KEYS];

	b8 mouse_down[PLATFORM_MAX_MOUSE_BUTTONS];
	int mouse_x, mouse_y;
	int mouse_dx, mouse_dy;

	b8 should_close;
} platform_state;

b8 platform_is_supported(void);
platform_window* platform_create_window(
 arena* arena, u32 width, u32 height, string8 title);

void platform_poll_events(platform_window* win, b8* should_close);
void platform_swap_buffers(platform_window* win);
void platform_close_window(platform_window*);
#endif // PLATFORM_H
