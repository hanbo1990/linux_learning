#include "touch_screen.h"

#include <stdlib.h>
#include <tslib.h>

#include "input_mgr/input_mgr.h"
#include "utils/custom_defines.h"

static struct tsdev* dev;

static int get_event(struct input_event* input_event)
{
    struct ts_sample sample;
    struct ts_event_data* ts_data;

    if (ts_read(dev, &sample, 1) != 1) {
        return -1;
    }

    ts_data = (struct ts_event_data*) malloc(sizeof(struct ts_event_data));
    if (NULL == ts_data) {
        LOG_ERROR("Malloc failure");
        return -1;
    }

    ts_data->x = sample.x;
    ts_data->y = sample.y;
    ts_data->pressure = sample.pressure;
    input_event->data = ts_data;
    input_event->time = sample.tv;
    input_event->type = EVENT_TYPE__TOUCH_SCREEN;

    return 0;
}

static int dev_init(void)
{
    dev = ts_setup(NULL, 0);
    if (!dev) {
        perror("ts_setup");
        return -1;
    }
    return 0;
}

static int dev_deinit(void)
{
    ts_close(dev);
    return 0;
}

static struct input_device device = {
    .name = "touchscreen",
    .get_event = get_event,
    .dev_init = dev_init,
    .dev_deinit = dev_deinit,
};

int touchscreen_init(void)
{
    return inputmgr_resigter(&device);
}
