
#include "display_mgr.h"

#include <string.h>

#include "lcd/lcd_display.h"
#include "utils/custom_defines.h"

static struct display_opr *display_op = NULL;
static struct display_opr *display_default = NULL;

static struct display_buf dis_buf;

int dismgr_register_display(struct display_opr *new_dis_op)
{
    RETURN_IF_NOT_SET_WITH_LOG(new_dis_op, "invalid input");

    new_dis_op->next = display_op;
    display_op = new_dis_op;

    return 0;
}

int dismgr_init(void)
{
    int ret = 0;
    struct display_opr *tmp = display_op;

    lcd_init();

    return ret;
}

int dismgr_deinit(void)
{
    struct display_opr *tmp;

    tmp = display_op;

    while (tmp) {
        (void) tmp->dev_deinit();
        tmp = tmp->next;
    }

    return 0;
}

int dismgr_set_default(char *name)
{
    struct display_opr *tmp = display_op;

    while (tmp) {
        if (strcmp(name, tmp->name) == 0) {
            display_default = tmp;
            return 0;
        }
        tmp = tmp->next;
    }

    return -1;
}

static int line_width_byte, pixel_width_byte;

int dismgr_init_default_display(void)
{
    if (display_default->dev_init()) {
        LOG_ERROR("failed to init device");
        return -1;
    }

    if (display_default->get_buf(&dis_buf)) {
        LOG_ERROR("failed to get buffer");
        return -1;
    }

    pixel_width_byte = dis_buf.bpp >> 3;
    line_width_byte = dis_buf.x_res * pixel_width_byte;

    return 0;
}

int dismgr_draw_pixel(int x, int y, unsigned int color)
{
    unsigned char *pen_8 = dis_buf.buf + y * line_width_byte + x * pixel_width_byte;
    unsigned int red, green, blue;
    unsigned short *pen_16;
    unsigned int *pen_32;

    pen_16 = (unsigned short *) pen_8;
    pen_32 = (unsigned int *) pen_8;

    switch (dis_buf.bpp) {
    case 8: {
        *pen_8 = color;
        break;
    }
    case 16: {
        /* 565 */
        red = (color >> 16) & 0xff;
        green = (color >> 8) & 0xff;
        blue = (color >> 0) & 0xff;
        color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        *pen_16 = color;
        break;
    }
    case 32: {
        *pen_32 = color;
        break;
    }
    default: {
        LOG_ERROR("can't surport %dbpp\n", dis_buf.bpp);
        break;
    }
    }
}

int dismgr_flush(struct region *region, struct display_buf *dis_buf)
{
    return display_default->flush_region(region, dis_buf);
}

struct display_buf *dismgr_get_display_buf(void)
{
    return &dis_buf;
}

void dismgr_draw_font_bitmap(struct font_bitmap *bitmap, int color)
{
    int i, j, p, q;
    int x, y, x_max, y_max, width;

    x = bitmap->region.left_up_point.x_pos;
    y = bitmap->region.left_up_point.y_pos;
    x_max = x + bitmap->region.width;
    y_max = y + bitmap->region.height;
    width = bitmap->region.width;

    for (j = y, q = 0; j < y_max; j++, q++) {
        for (i = x, p = 0; i < x_max; i++, p++) {
            if (i < 0 || j < 0 || i >= dis_buf.x_res || j >= dis_buf.y_res) {
                continue;
            }
            if (bitmap->bitmap_buf[q * width + p]) {
                dismgr_draw_pixel(i, j, color);
            } else {
                dismgr_draw_pixel(i, j, 0);
            }
        }
    }
}