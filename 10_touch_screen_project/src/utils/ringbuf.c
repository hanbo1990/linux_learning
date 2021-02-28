
#include "utils/ringbuf.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define INDEX_MEM_LOCATION(pbuf, idx) (pbuf->buf_head + pbuf->item_size * idx)

int ringbuf_push(struct ring_buf *buf, void *data)
{
    if (NULL == buf || NULL == data) {
        RING_BUF_LOG("invalid input\n");
        return -1;
    }

    if (buf->__item_count == buf->num_item_max) {
        RING_BUF_LOG("buffer is full\n");
        return -1;
    }

    RING_BUF_LOG("writing to index %d\n", buf->idx_write);
    memcpy(INDEX_MEM_LOCATION(buf, buf->idx_write), data, buf->item_size);
    buf->idx_write = (buf->idx_write + 1) % buf->num_item_max;
    buf->__item_count++;

    return 0;
}

int ringbuf_pop(struct ring_buf *buf, void *data)
{
    if (NULL == buf || NULL == data) {
        RING_BUF_LOG("invalid input\n");
        return -1;
    }

    if (0 == buf->__item_count) {
        RING_BUF_LOG("buffer is empty\n");
        return -1;
    }

    RING_BUF_LOG("reading from index %d\n", buf->idx_read);
    memcpy(data, INDEX_MEM_LOCATION(buf, buf->idx_read), buf->item_size);
    buf->idx_read = (buf->idx_read + 1) % buf->num_item_max;
    buf->__item_count--;

    return 0;
}