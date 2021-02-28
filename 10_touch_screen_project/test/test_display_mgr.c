#include "display_mgr/display_mgr.h"
#include "font_8x16.c.inc"

void lcd_put_ascii(int x, int y, unsigned char c, unsigned int color)
{
    unsigned char *dots = (unsigned char *) &fontdata_8x16[c * 16];
    int i, b;
    unsigned char byte;

    for (i = 0; i < 16; i++) {
        byte = dots[i];
        for (b = 7; b >= 0; b--) {
            if (byte & (1 << b)) {
                /* show */
                dismgr_draw_pixel(x + 7 - b, y + i, color);
            } else {
                /* hide */
                dismgr_draw_pixel(x + 7 - b, y + i, 0);
            }
        }
    }
}

int main(int argc, char **argv)
{
    struct region region = {
        .left_up_point =
            {
                .x_pos = 0,
                .y_pos = 0,
            },
        .height = 100,
        .width = 100,
    };
    struct display_buf *dis_buf;

    (void) dismgr_init();
    dismgr_set_default("fb");
    dismgr_init_default_display();
    lcd_put_ascii(500, 500, 'C', 0x009900);
    dis_buf = dismgr_get_display_buf();
    dismgr_flush(&region, dis_buf);
}
