#include "arena.h"
#include "macros.h"
#include "profile.h"
#include "spark.h"
#include "string8.h"
#include <stdio.h>

#define WINDOW_W 1200
#define WINDOW_H 1200

int main(void) {
	begin_profile();
	begin_time_function;
	arena *engine_arena = arena_create(KiB(64));
	string8 title = string8_lit("Spark Engine");
	spark_window *window = spark_create_window(engine_arena, WINDOW_W, WINDOW_H, title);

	if(window == NULL) {
		string8 err = string8_lit("An error occured: cannot open display.\n");
		write(1, err.data, err.length);
		return -1;
	}

	// clang-format off
	f32 vertices[] = {
		-0.5f, -0.5f, 0.0f,  // bottom left;
		 0.0f,  0.5f, 0.0f, // top middle,
		 0.5f, -0.5f, 0.0f // bottom right

	};
	// clang-format on

	while(!spark_window_should_close(window)) {
		begin_time_block("Game Loop");
		spark_poll_events(window);
		glClearColor(33.0 / 255.0, 33.0 / 255.0, 33.0 / 255.0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		spark_swap_buffers(window);
		end_time_block;
	}

	spark_close_window(window);
	end_time_function;
	end_profile();
	return 0;
}
