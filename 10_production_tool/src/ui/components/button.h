#ifndef _BUTTON_H
#define _BUTTON_H

#include "display_mgr/display_mgr.h"
#include "input_mgr/input_mgr.h"
#include "utils/custom_defines.h"

#define COLOR_DEFAULT_BUTTON  0xFF0000
#define COLOR_BTN_PRESSED     0x00FF00
#define COLOR_DEFAULT_TEXT    0x000000
#define COLOR_CMD_IN_PROGRESS 0x0000FF
#define COLOR_CMD_START       0x550055

struct button;

typedef int (*button_on_draw)(struct button *button, struct display_buf *buf);
typedef int (*button_on_press)(struct button *button, struct display_buf *buf,
                               struct input_event *event);

struct button {
    char *name;
    char *text;
    int is_pressed;
    struct region region;
    button_on_draw on_draw;
    button_on_press on_press;
};

void button_init(struct button *button, char *name, char *text, struct region *region,
                 button_on_draw on_draw, button_on_press on_press);

#endif /* _BUTTON_H */
