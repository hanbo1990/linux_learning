#ifndef _RING_BUF_H
#define _RING_BUF_H

// #define LOG_ENABLED

#ifdef LOG_ENABLED
    #define RING_BUF_LOG
#endif

struct ring_buf{
    char* name;
    int num_item_max;
    int item_size;
    int idx_read;
    int idx_write;
    int __is_full;
    int __item_count; /* use this to make use of all buffer areas */
    void *buf_head;
};

#define RING_BUF_NAME(alias) alias##_ring_buf
#define RING_BUF_DEF(alias, max_count, per_size) \
    char alias##_data[max_count * per_size];\
    struct ring_buf alias##_ring_buf = {\
        .name = #alias,\
        .num_item_max = max_count,\
        .item_size = per_size,\
        .idx_read = 0,\
        .idx_write = 0,\
        .__is_full = 0,\
        .__item_count = 0,\
        .buf_head = alias##_data,\
    };

#define RING_BUF_DEF_STATIC(alias, max_count, per_size) \
    static char alias##_data[max_count * per_size];\
    static struct ring_buf alias##_ring_buf = {\
        .name = #alias,\
        .num_item_max = max_count,\
        .item_size = per_size,\
        .idx_read = 0,\
        .idx_write = 0,\
        .__is_full = 0,\
        .__item_count = 0,\
        .buf_head = alias##_data,\
    };

/**
 * @brief Push item to ring buffer, ring buffer will copy the data
 * 
 * @param buf[in] pointer to the ring buffer
 * @param data[in] pointer to data to push
 * @return 0 if success and 1 if fail
 */
int ringbuf_push(struct ring_buf *buf, void *data);

/**
 * @brief Pop item from ring buffer
 * 
 * @param buf [in] pointer to the ring buffer
 * @param data [out] pointer to the buffer to receive output
 * @return 0 if success and 1 if fail
 */
int ringbuf_pop(struct ring_buf *buf, void *data);

#endif /* _RING_BUF_H */
