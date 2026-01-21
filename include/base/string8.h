#ifndef STRING8_H
#define STRING8_H
#include "arena.h"
#include "types.h"

#define STRING8_MAX 1024

typedef struct {
    u8 *data;
    size_t length;
} string8;

string8 string8_lit(char *str);
string8 string8_make(arena *a, char *cstring);
string8 string8_read_file(arena *a, int fd);

#endif // STRING8_H
