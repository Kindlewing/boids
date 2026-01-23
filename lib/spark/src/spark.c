#include "spark.h"
#include "base/string8.h"
#include "base/typedefs.h"

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

void spark_close_window(spark_window *win) {
	platform_close_window(win);
}

