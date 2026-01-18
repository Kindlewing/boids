#include "platform.h"
#include <GL/gl.h>
#include <stdbool.h>

int main(void) {
	arena *engine_arena = arena_create(1024 * 1024);
	platform_window *win = platform_create_window(engine_arena, 800, 800, "GUI");

	glEnable(GL_DEPTH_TEST);

	bool should_close = false;

	while(!should_close) {
		platform_poll_events(win, &should_close);

		glClearColor(0.117f, 0.117f, 0.180f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		platform_swap_buffers(win);
	}
	platform_close_window(win);
	arena_free(engine_arena);
	return 0;
}
