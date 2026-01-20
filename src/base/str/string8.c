#include "base/string8.h"

string8 string8_lit(char *str) {
	string8 s;
	s.data = (u8 *)str;
	s.length = 0;
	while(str[s.length] != 0) {
		s.length++;
	}
	return s;
}

string8 string8_make(arena *a, char *cstring) {
	string8 s;
	s.length = 0;
	while(cstring[s.length]) {
		s.length++;
	}

	s.data = arena_push_array(a, u8, s.length);
	for(size_t i = 0; i < s.length; i++) {
		s.data[i] = (u8)cstring[i];
	}
	return s;
}
