#include "arena.h"
#include "spark.h"
#include "string8.h"
#include "typedefs.h"
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>

spark_window *platform_create_window(arena *a, u32 w, u32 h, string8 title);
b8 platform_window_should_close(spark_window *win);

void platform_poll_events(spark_window *win);
void platform_swap_buffers(spark_window *win);
void platform_close_window(spark_window *win);
