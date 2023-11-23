#pragma once
#include <stdio.h>

// memory
// debug
// info
// error
// fatal

#ifdef NDEBUG
    #define LOGM(...)
    #define LOGD(...)
    #define LOGI(...)
    #define LOGE(...) fprintf(stderr, "E: "__VA_ARGS__)
    #define LOGF(...) fprintf(stderr, "F: "__VA_ARGS__)
#else
    #define LOGM(...) fprintf(stderr, "M: "__VA_ARGS__)
    #define LOGD(...) fprintf(stderr, "D: "__VA_ARGS__)
    #define LOGI(...) fprintf(stderr, "I: "__VA_ARGS__)
    #define LOGE(...) fprintf(stderr, "E: "__VA_ARGS__)
    #define LOGF(...) fprintf(stderr, "F: "__VA_ARGS__)
#endif
