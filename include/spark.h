#ifndef SPARK_H
#define SPARK_H
#include "base/arena.h"
#include "base/string8.h"

typedef struct spark_window spark_window;

spark_window *spark_create_window(arena *a, u32 width, u32 height, string8 name);
void spark_swap_buffers(spark_window *window);
void spark_poll_events(spark_window *win, b8 *should_close);
void spark_close_window(spark_window *win);

#endif // !SPARK_H
