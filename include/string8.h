#ifndef STRING8_H
#define STRING8_H
#include "arena.h"
#include "typedefs.h"

#define STRING8_MAX 1024

typedef struct {
	u8 *data;
	size_t length;
} string8;

string8 string8_lit(char *str);
string8 *string8_from(arena *a, char *cstring);

#endif // STRING8_H
