#include "spark/spark.h"
#include "spark/platform/platform.h"

arena *g_spark_arena = NULL;
#define SPARK_ALLOC(type) (type *)arena_push(g_spark_arena, sizeof(type))

spark_window *spark_create_window(arena *a, u32 width, u32 height, string8 title) {
	g_spark_arena = a;
	return spark_platform.create_window(width, height, title);
}
void spark_poll_events(spark_window *win) { return spark_platform.poll_events(win); }
