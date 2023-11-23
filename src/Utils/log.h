#pragma once
#include <stdio.h>

#define LOGM(...) fprintf(stderr, "M: "__VA_ARGS__)  // memory
#define LOGD(...) fprintf(stderr, "D: "__VA_ARGS__)  // debug
#define LOGI(...) fprintf(stderr, "I: "__VA_ARGS__)  // info
#define LOGE(...) fprintf(stderr, "E: "__VA_ARGS__)  // error
#define LOGF(...) fprintf(stderr, "F: "__VA_ARGS__)  // fatal
