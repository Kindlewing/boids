#include "string8.h"

string8 string8_lit(char *str) {
	string8 s;
	s.data = (u8 *)str;
	s.length = 0;

	while(str[s.length] != 0) {
		s.length++;
	}

	return s;
}

string8 *string8_from(arena *a, char *cstring) {
	string8 s = {0};
	size_t len = 0;
	while(cstring[len] != '\0') {
		s.data[len] = cstring[len];
		len += 1;
	}
	s.length = len;
	return s;
}
