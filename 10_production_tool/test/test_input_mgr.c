#include "input_mgr/input_mgr.h"
#include "utils/custom_defines.h"

int main(int argc, char** argv)
{
    struct input_event input_event;
    struct ts_event_data* ts_data;
    struct report_event_data* report_data;
    inputmgr_init();
    inputmgr_init_all_devices();
    while (1) {
        if (!inputmgr_get_event(&input_event)) {
            if (input_event.type == EVENT_TYPE__TOUCH_SCREEN) {
                ts_data = (struct ts_event_data*) input_event.data;
                LOG_INFO("[%ld] ts data received: x = %d, y = %d, pressure = %d",
                         input_event.time.tv_sec, ts_data->x, ts_data->y, ts_data->pressure);
            } else if (input_event.type == EVENT_TYPE__STR_REPORT) {
                report_data = (struct report_event_data*) input_event.data;
                LOG_INFO("received str data %s", report_data->str);
                report_data->data_used(report_data);
            } else {
                LOG_INFO("WHAT!");
            }
        } else {
            LOG_INFO("WHAT!");
        }
    }
}
