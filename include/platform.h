#ifndef PLATFORM_H
#define PLATFORM_H
#include "arena.h"
#include <stdbool.h>

#define PLATFORM_MAX_KEYS 256
#define PLATFORM_MAX_MOUSE_BUTTONS 8

typedef struct platform_window platform_window;

typedef struct {
	bool keys_down[PLATFORM_MAX_KEYS];
	bool keys_pressed[PLATFORM_MAX_KEYS];
	bool keys_released[PLATFORM_MAX_KEYS];

	bool mouse_down[PLATFORM_MAX_MOUSE_BUTTONS];
	int mouse_x, mouse_y;
	int mouse_dx, mouse_dy;

	bool should_close;
} platform_state;

bool platform_is_supported(void);
platform_window *platform_create_window(arena *arena, int width, int height, const char *title);

void platform_poll_events(platform_window *win, bool *should_close);
void platform_swap_buffers(platform_window *win);
void platform_close_window(platform_window *);
#endif // PLATFORM_H
