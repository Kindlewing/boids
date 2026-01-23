#include "str/string8.h"

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

string8 string8_read_file(arena *a, int fd) {
	string8 s = {
		0
	};

	i64 size = lseek(fd, 0, SEEK_END);

	lseek(fd, 0, SEEK_SET);

	if(size <= 0) {
		return s;
	}

	s.data = arena_push_array(a, u8, size);
	s.length = size;

	i64 read_bytes = read(fd, s.data, size);
	if(read_bytes != size) {
		// handle error if you care
	}
	return s;
}
