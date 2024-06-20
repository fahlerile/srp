#pragma once

#include <time.h>

#define TIMER_START(name) clock_t name = clock(), name##_diff
#define TIMER_STOP(name) name##_diff = clock() - name

#define TIMER_REPORT(name, type, multiplier) \
	((type) name##_diff * (multiplier) / CLOCKS_PER_SEC)
#define TIMER_REPORT_S(name, type)	TIMER_REPORT(name, type, 1)
#define TIMER_REPORT_MS(name, type) TIMER_REPORT(name, type, 1000)
#define TIMER_REPORT_US(name, type) TIMER_REPORT(name, type, 1000000)

#define TIME_SECTION(var, type, multiplier, section) \
	{ \
		TIMER_START(_1); \
		section \
		TIMER_STOP(_1); \
		var = TIMER_REPORT(_1, type, (multiplier)); \
	}
#define TIME_SECTION_S(var, type, section) TIME_SECTION(var, type, 1, section)
#define TIME_SECTION_MS(var, type, section) TIME_SECTION(var, type, 1000, section)
#define TIME_SECTION_US(var, type, section) TIME_SECTION(var, type, 1000000, section)

