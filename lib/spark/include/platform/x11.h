#include "mem/arena.h"
#include "spark.h"
#include "str/string8.h"
#include "typedefs.h"

spark_window *platform_create_window(arena *a, u32 w, u32 h, string8 title);
void platform_poll_events(spark_window *win);
void platform_close_window(spark_window *win);
