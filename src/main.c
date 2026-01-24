#include "mem/arena.h"
#include "spark.h"
#include "str/string8.h"

#define WINDOW_W 1200
#define WINDOW_H 1200

int main(void) {
    arena *engine_arena = arena_create(1024);
    string8 title = string8_lit("Spark Engine");
    spark_window *window =
	    spark_create_window(engine_arena, WINDOW_W, WINDOW_H, title);

    if (window == NULL) {
	string8 err = string8_lit("An error occured: cannot open X display.\n");
	write(1, err.data, err.length);
	return -1;
    }

    while (true) {
	spark_poll_events(window);
    }
    spark_close_window(window);
    return (0);
}
