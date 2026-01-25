#ifndef MACROS_H
#define MACROS_H
#include "types.h"

enum {
	KiB_1 = 1ULL << 10,
	MiB_1 = 1ULL << 20,
	GiB_1 = 1ULL << 30,
};

#define KiB(x) ((u64)(x) * KiB_1)
#define MiB(x) ((u64)(x) * MiB_1)
#define GiB(x) ((u64(x) * GiB_1)

#endif // MACROS_H
