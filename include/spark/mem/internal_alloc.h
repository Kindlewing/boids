#include "base/arena.h"

/* defined in spark.c */
extern arena *g_spark_arena;

#define SPARK_ALLOC(type) ((type *)arena_push(g_spark_arena, sizeof(type)))
#define SPARK_ALLOC_ZERO(type) ((type *)arena_push_zero(g_spark_arena, sizeof(type)))
