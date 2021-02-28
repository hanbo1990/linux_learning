#include <stdio.h>

#include "utils/ringbuf.h"

struct test_data {
    int a;
    char str[3];
};

RING_BUF_DEF(aaatest, 5, sizeof(int));
RING_BUF_DEF(test_item, 3, sizeof(struct test_data));

int main(int argc, char** argv)
{
    int a[6] = {1, 2, 3, 4, 5, 6};
    int result;
    for (int i = 0; i < 6; i++) {
        if (!ringbuf_push(&RING_BUF_NAME(aaatest), (void*) &a[i]))
            printf("data %d pushed \n", a[i]);
    }

    for (int i = 0; i < 1; i++) {
        if (!ringbuf_pop(&RING_BUF_NAME(aaatest), (void*) &result))
            printf("result is %d\n", result);
    }

    for (int i = 0; i < 4; i++) {
        if (!ringbuf_push(&RING_BUF_NAME(aaatest), (void*) &a[i]))
            printf("data %d pushed \n", a[i]);
    }

    struct test_data result_struct;
    struct test_data input = {.a = 0, .str = "ha"};

    for (int i = 0; i < 6; i++) {
        input.a = i + 50;
        if (!ringbuf_push(&RING_BUF_NAME(test_item), (void*) &input))
            printf("data %d pushed \n", input.a);
    }

    for (int i = 0; i < 1; i++) {
        if (!ringbuf_pop(&RING_BUF_NAME(test_item), (void*) &result_struct))
            printf("result is %d\n", result_struct.a);
    }

    for (int i = 0; i < 4; i++) {
        input.a = 5 * i;
        if (!ringbuf_push(&RING_BUF_NAME(test_item), (void*) &input))
            printf("data %d pushed \n", input.a);
    }

    for (int i = 0; i < 7; i++) {
        if (!ringbuf_pop(&RING_BUF_NAME(test_item), (void*) &result_struct))
            printf("result is %d\n", result_struct.a);
    }

    return 0;
}