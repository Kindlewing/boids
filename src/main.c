#include "base/arena.h"
#include "base/string8.h"
#include "spark/spark.h"
#include <X11/Xlib.h>

#define WINDOW_W 1200
#define WINDOW_H 1200

int main(void) {
	arena *engine_arena = arena_create(1024);
	string8 title = string8_lit("Spark Engine");
	spark_window *window = spark_create_window(engine_arena, WINDOW_W, WINDOW_H, title);
	while(true) {
		spark_poll_events(window);
	}
	return 0;
}
