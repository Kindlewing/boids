#ifndef SPARK_H
#define SPARK_H
#include "mem/arena.h"
#include "str/string8.h"

typedef struct spark_window spark_window;

spark_window *spark_create_window(arena *a, u32 width, u32 height,
                                  string8 title);
void spark_swap_buffers(spark_window *window);
void spark_poll_events(spark_window *win);
void spark_close_window(spark_window *win);

#endif // !SPARK_H
