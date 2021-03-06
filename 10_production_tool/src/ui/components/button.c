
#include "button.h"

#include <string.h>

int default_on_draw(struct button *button, struct display_buf *buf)
{
    dismgr_draw_region(&button->region, COLOR_DEFAULT_BUTTON);
    dismgr_draw_text_in_region(button->text, &button->region, COLOR_DEFAULT_TEXT);
    dismgr_flush(&button->region, buf);
    return 0;
}

int default_on_press(struct button *button, struct display_buf *buf, struct input_event *event)
{
    unsigned int color = COLOR_DEFAULT_BUTTON;
    button->is_pressed = !button->is_pressed;
    if (button->is_pressed) {
        color = COLOR_BTN_PRESSED;
    }

    dismgr_draw_region(&button->region, color);
    dismgr_draw_text_in_region(button->text, &button->region, COLOR_DEFAULT_TEXT);
    dismgr_flush(&button->region, buf);
    return 0;
}

void button_init(struct button *button, char *name, char *text, struct region *region,
                 button_on_draw on_draw, button_on_press on_press)
{
    button->name = name;
    button->text = text;
    button->is_pressed = 0;
    memcpy(&button->region, region, sizeof(struct region));
    button->on_draw = on_draw ? on_draw : default_on_draw;
    button->on_press = on_press ? on_press : default_on_press;
}
