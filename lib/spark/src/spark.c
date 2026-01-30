#include "spark.h"
#include "string8.h"
#include "typedefs.h"

#if defined(__linux__)
#include "platform/x11.h"
#else
#error "Unsupported platform"
#endif

spark_window *spark_create_window(arena *a, u32 width, u32 height, string8 title) {
	return platform_create_window(a, width, height, title);
}

void spark_poll_events(spark_window *win) {
	platform_poll_events(win);
}

b8 spark_window_should_close(spark_window *win) {
	return platform_window_should_close(win);
}

void spark_swap_buffers(spark_window *window) {
	platform_swap_buffers(window);
}

void spark_close_window(spark_window *win) {
	platform_close_window(win);
}
