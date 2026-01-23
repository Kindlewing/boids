#ifndef PLATFORM_H
#define PLATFORM_H
#include "base/string8.h"
#include "base/types.h"
#include "spark/spark.h"

typedef struct spark_platform_api {
  spark_window *(*create_window)(u32, u32, string8);
  void (*close_window)(spark_window *);
  void (*poll_events)(spark_window *);
} spark_platform_api;

extern spark_platform_api spark_platform;
#endif
