#include "profile.h"
#include "sys/time.h"
#include "typedefs.h"

static profiler global_prof;

void begin_profile() { global_prof.start_tsc = __rdtsc(); }

static u64 get_timer_freq(void) { return 1000000; }

static u64 read_os_timer(void) {
	struct timeval time_value;
	gettimeofday(&time_value, 0);
	u64 res = get_timer_freq() * (u64)time_value.tv_sec + (u64)time_value.tv_usec;
	return res;
}

void init_profile_block(profile_block *block, string8 name, u32 idx) { block->label = name; }

void destroy_profile_block(profile_block *block) {}

void end_profile() {}
