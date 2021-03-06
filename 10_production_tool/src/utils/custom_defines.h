#ifndef _CUSTOM_DEFINES_H
#define _CUSTOM_DEFINES_H

#include <stdlib.h>

#include "utils/logger.h"

#ifndef NULL
    #define NULL ((void*) 0)
#endif

struct region {
    struct {
        int x_pos;
        int y_pos;
    } left_up_point;

    int width;
    int height;
};

struct font_bitmap {
    struct region region;

    /**
     *  to support freetype ...etc
     */
    struct {
        int cur_origin_x;
        int cur_origin_y;
        int next_origin_x;
        int next_origin_y;
    } origin;
    unsigned char* bitmap_buf;
};

#define RETURN_IF_NOT_SET_WITH_LOG(status, fmt, args...)                                           \
    if (!status) {                                                                                 \
        LOG_WARN("fmt", ##args);                                                                   \
        return -1;                                                                                 \
    }

#define RETURN_IF_NOT_SET(status)                                                                  \
    if (!status) {                                                                                 \
        return -1;                                                                                 \
    }

static inline void custom_free(void* ptr)
{
    if (ptr) {
        free(ptr);
    }
}

static inline void custom_free_and_set_null(void** pptr)
{
    if (*pptr) {
        free(*pptr);
        *pptr = NULL;
    }
}

#endif /* _CUSTOM_DEFINES_H */
