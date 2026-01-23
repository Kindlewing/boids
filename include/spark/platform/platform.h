#ifndef PLATFORM_H
#define PLATFORM_H
#include "base/string8.h"
#include "base/types.h"

typedef struct spark_window spark_window;

typedef struct spark_platform_api {
	spark_window *(*create_window)(i32, i32, string8);
	void (*destroy_window)(spark_window *);
	void (*poll_events)(void);
} spark_platform_api;

extern spark_platform_api spark_platform;
#endif
