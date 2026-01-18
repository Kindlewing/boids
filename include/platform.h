#ifndef PLATFORM_H
#define PLATFORM_H
#include "arena.h"
#include <stdbool.h>

typedef struct platform_window platform_window;

bool platform_is_supported(void);
platform_window *platform_create_window(arena *arena, int width, int height,
                                        const char *title);
void platform_poll_events(platform_window *win, bool *should_close);
void platform_swap_buffers(platform_window *win);
void platform_close_window(platform_window *);
#endif // PLATFORM_H
