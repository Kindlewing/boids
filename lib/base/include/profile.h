#ifndef PROFILE_H
#define PROFILE_H
#include "string8.h"
#include "typedefs.h"
#include <x86intrin.h>

#define PROFILE_MAX_ANCHORS 4096

typedef struct {
	u64 tsc_elapsed;
	u64 times_hit; // how many times have we called this?
	string8 label;
} profile_anchor;

typedef struct {
	u64 start_tsc;
	u32 anchor_index; // function/scope this lives in
} profile_block;

typedef struct {
	profile_anchor anchors[PROFILE_MAX_ANCHORS]; // a list of scopes
	u64 start_tsc;
	u64 end_tsc;
} profiler;

void begin_profile(void);
void init_profile_block(profile_block *block, string8 name, u32 idx);
void destroy_profile_block(profile_block *block);
void end_profile(void);
#endif
