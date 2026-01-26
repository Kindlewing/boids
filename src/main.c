#include "arena.h"
#include "macros.h"
#include "math.h"
#include "spark.h"
#include "string8.h"
#include <stdio.h>

#define WINDOW_W 1200
#define WINDOW_H 1200

int main(void) {
	arena *engine_arena = arena_create(KiB(64));
#if defined(_WIN32)
	return 0;
#endif
	string8 title = string8_lit("Spark Engine");
	spark_window *window = spark_create_window(engine_arena, WINDOW_W, WINDOW_H, title);

	if (window == NULL) {
		string8 err = string8_lit("An error occured: cannot open X display.\n");
		write(1, err.data, err.length);
		return -1;
	}

	while (true) {
		spark_poll_events(window);
		glClearColor(33.0 / 255.0, 33.0 / 255.0, 33.0 / 255.0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		spark_swap_buffers(window);
	}
	spark_close_window(window);
	return (0);
}
