#include "profile.h"
#include "sys/time.h"
#include "typedefs.h"
#include <stdio.h>

static profiler global_prof;
void begin_profile() { global_prof.start_tsc = __rdtsc(); }
static u64 get_timer_freq(void) { return 1000000; }
static u64 read_os_timer(void) {
	struct timeval time_value;
	gettimeofday(&time_value, 0);
	u64 res = get_timer_freq() * (u64)time_value.tv_sec + (u64)time_value.tv_usec;
	return res;
}
static u64 estimate_cpu_timer_freq(void) {
	u64 ms_to_wait = 100;
	u64 os_freq = get_timer_freq();
	u64 cpu_start = __rdtsc();
	u64 os_start = read_os_timer();
	u64 os_elapsed = 0;
	u64 os_wait = os_freq * ms_to_wait / 1000;
	while(os_elapsed < os_wait) {
		u64 os_now = read_os_timer();
		os_elapsed = os_now - os_start;
	}
	u64 cpu_end = __rdtsc();
	u64 cpu_elapsed = cpu_end - cpu_start;
	u64 cpu_freq = 0;
	if(os_elapsed) {
		cpu_freq = os_freq * cpu_elapsed / os_elapsed;
	}
	return cpu_freq;
}
void init_profile_block(profile_block *block, string8 name, u32 idx) {
	block->start_tsc = __rdtsc();
	block->anchor_index = idx;
	profile_anchor *a = &global_prof.anchors[idx];
	a->label = name;
}
void destroy_profile_block(profile_block *block) {
	u64 elapsed = __rdtsc() - block->start_tsc;
	profile_anchor *a = &global_prof.anchors[block->anchor_index];
	a->tsc_elapsed += elapsed;
	a->times_hit += 1;
}
void end_profile() {
	global_prof.end_tsc = __rdtsc();
	u64 cpu_freq = estimate_cpu_timer_freq();
	if(cpu_freq == 0) {
		return;
	}
	u64 total_cycles = global_prof.end_tsc - global_prof.start_tsc;
	f64 total_seconds = (f64)total_cycles / (f64)cpu_freq;
	printf("Total time: %.6f ms\n\n", total_seconds * 1000.0);
	for(u32 i = 0; i < PROFILE_MAX_ANCHORS; ++i) {
		profile_anchor *a = &global_prof.anchors[i];
		if(a->times_hit == 0) {
			continue;
		}
		f64 seconds = (f64)a->tsc_elapsed / (f64)cpu_freq;
		f64 avg = seconds / (f64)a->times_hit;
		printf("%-30.*s | hits: %6llu | total: %8.3f ms | avg: %8.3f us\n", (int)a->label.length,
			   a->label.data, (unsigned long long)a->times_hit, seconds * 1000.0, avg * 1000000.0);
	}
}
