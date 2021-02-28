#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/input.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tslib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    struct tsdev* dev;
    struct input_absinfo slot;
    struct ts_sample_mt** samp_mt;
    struct ts_sample_mt* temp_mt;
    int ret;
    int i;

    int max_slots;

    dev = ts_setup(NULL, 0);
    if (!dev) {
        perror("ts_setup");
        return errno;
    }

    if (ioctl(ts_fd(dev), EVIOCGABS(ABS_MT_SLOT), &slot) < 0) {
        perror("ioctl EVIOGABS");
        ts_close(dev);
        return errno;
    }

    max_slots = slot.maximum + 1 - slot.minimum;
    printf("max supported slots %d\n", max_slots);

    samp_mt = malloc(sizeof(struct ts_sample_mt*));
    if (!samp_mt) {
        ts_close(dev);
        return -ENOMEM;
    }

    samp_mt[0] = calloc(max_slots, sizeof(struct ts_sample_mt));
    if (!samp_mt[0]) {
        free(samp_mt);
        ts_close(dev);
        return -ENOMEM;
    }

    while (1) {
        ret = ts_read_mt(dev, samp_mt, max_slots, 1);
        if (ret < 0) {
            perror("ts_read_mt");
            ts_close(dev);
            return -1;
        }

        for (i = 0; i < max_slots; i++) {
            temp_mt = &samp_mt[0][i];
            if (temp_mt->valid) {
                if (-1 == temp_mt->tracking_id)
                    printf("x: %d, y: %d released on slot %d\n", temp_mt->x, temp_mt->y, temp_mt->slot);
                else
                    printf("x: %d, y: %d pressed on slot %d\n", temp_mt->x, temp_mt->y, temp_mt->slot);
            }
        }
    }

    return 0;
}