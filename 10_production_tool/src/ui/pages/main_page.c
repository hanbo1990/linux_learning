#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "display_mgr/display_mgr.h"
#include "ui/components/button.h"
#include "ui/ui_mgr.h"
#include "utils/custom_defines.h"
#include "utils/modules.h"

#define X_GAP 5U
#define Y_GAP 5U

static struct button btns[CONFIG_MAX_NUMBER];
static int btn_number;

static int btn_on_press(struct button* button, struct display_buf* buf, struct input_event* event)
{
    int color = COLOR_DEFAULT_BUTTON;
    struct report_event_data* report_data;
    char name[100], status[100];
    char* but_display;
    struct config_item* tmp_config;
    int pid;

    tmp_config = config_get_item_by_name(button->name);

    if (NULL == tmp_config) {
        LOG_ERROR("strange thing happens! cannot load config by button name");
        return -1;
    }

    if (EVENT_TYPE__TOUCH_SCREEN == event->type) {
        if (tmp_config->can_be_press == 0) {
            return -1;
        }

        /* if command exist, color is set to command start, waitting color update from net */
        if (strlen(tmp_config->command) != 0) {
            /* todo: add timeout for command, and also command once start, don't start again */
            LOG_INFO("running command %s", tmp_config->command);
            pid = fork();

            if (!pid) {
                system(tmp_config->command);
                color = COLOR_CMD_START;
            }

        } else {
            button->is_pressed = !button->is_pressed;
            if (button->is_pressed) {
                color = COLOR_BTN_PRESSED;
            }
        }
        but_display = button->name;
    } else if (EVENT_TYPE__STR_REPORT == event->type) {
        report_data = (struct report_event_data*) event->data;
        sscanf(report_data->str, "%s %s", name, status);
        but_display = button->name;
        if (strcmp(status, "ok") == 0) {
            color = COLOR_BTN_PRESSED;
        } else if (status[0] >= '0' && status[0] <= '9') {
            color = COLOR_CMD_IN_PROGRESS;
            but_display = status;
        }
    } else {
        return -1;
    }

    dismgr_draw_region(&button->region, color);
    dismgr_draw_text_in_region(but_display, &button->region, COLOR_DEFAULT_TEXT);
    dismgr_flush(&button->region, buf);
    return 0;
}

static void generate_buttons(void)
{
    int btn_count, btn_columns, btn_rows;
    int width, height;
    struct display_buf* dis_buf;
    int xres, yres, start_x, start_y, pre_start_x, pre_start_y;
    struct region btn_region;
    int i, j, n;
    struct button* btn;

    btn_count = config_get_item_count();
    dis_buf = dismgr_get_display_buf();
    xres = dis_buf->x_res;
    yres = dis_buf->y_res;

    width = sqrt(0.618 * xres * yres / btn_count);
    btn_columns = xres / width + 1;
    width = xres / btn_columns;
    height = width * 0.618;

    start_x = (xres - width * btn_columns) / 2;
    btn_rows = btn_count / btn_columns;
    if (btn_rows * btn_columns < btn_count) {
        btn_rows++;
    }
    start_y = (yres - btn_rows * height) / 2;

    n = 0;
    for (i = 0; i < btn_rows && n < btn_count; i++) {
        pre_start_y = start_y + i * height;
        pre_start_x = start_x - width;

        for (j = 0; j < btn_columns && n < btn_count; j++) {
            btn = &btns[n];
            btn_region.left_up_point.x_pos = pre_start_x + width;
            btn_region.left_up_point.y_pos = pre_start_y;
            btn_region.width = width - X_GAP;
            btn_region.height = height - Y_GAP;
            pre_start_x = btn_region.left_up_point.x_pos;
            button_init(btn, config_get_item_by_index(n)->name, config_get_item_by_index(n)->name,
                        &btn_region, NULL, btn_on_press);
            n++;
        }
    }

    btn_number = n;

    for (i = 0; i < n; i++) { btns[i].on_draw(&btns[i], dis_buf); }
}

static int is_touch_point_in_region(int x, int y, struct region* region)
{
    if (x < region->left_up_point.x_pos || x > region->left_up_point.x_pos + region->width) {
        return 0;
    }
    if (y < region->left_up_point.y_pos || y > region->left_up_point.y_pos + region->height) {
        return 0;
    }
    return 1;
}

static struct button* get_button_by_event(struct input_event* event)
{
    int i;
    struct ts_event_data* touch_data;
    struct report_event_data* report_data;
    char name[100];

    if (EVENT_TYPE__TOUCH_SCREEN == event->type) {
        touch_data = (struct ts_event_data*) event->data;
        if (touch_data->pressure) {
            for (i = 0; i < btn_number; i++) {
                if (is_touch_point_in_region(touch_data->x, touch_data->y, &btns[i].region)) {
                    return &btns[i];
                }
            }
        }
        return NULL;
    } else if (EVENT_TYPE__STR_REPORT == event->type) {
        report_data = (struct report_event_data*) event->data;
        sscanf(report_data->str, "%s", name);

        for (i = 0; i < btn_number; i++) {
            if (strcmp(name, btns[i].name) == 0) {
                return &btns[i];
            }
        }
        report_data->data_used(report_data);
        return NULL;
    } else {
        return NULL;
    }
}

void run_page(void* params)
{
    int err;
    struct input_event event;
    struct button* btn;

    /* load configuration file */
    err = config_parse_file();
    if (err) {
        LOG_ERROR("Failed to load configuration file");
    }

    /* generate page */
    generate_buttons();

    while (1) {
        /* read input event */
        err = inputmgr_get_event(&event);
        if (err) {
            continue;
        }

        /* find button based on input event*/
        btn = get_button_by_event(&event);
        if (NULL == btn) {
            continue;
        }
        /* button on press */
        btn->on_press(btn, dismgr_get_display_buf(), &event);
    }
}

static struct page main_page = {
    .name = "main",
    .run = run_page,
};

static int mainpage_init(void)
{
    uimgr_resiter_page(&main_page);
    return 0;
}

MODULE_EXPORT(mainpage_init);
