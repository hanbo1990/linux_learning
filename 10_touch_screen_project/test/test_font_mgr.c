#include "display_mgr/display_mgr.h"
#include "font_8x16.c.inc"
#include "font_mgr/font_mgr.h"

int main(int argc, char **argv)
{
    struct display_buf *dis_buf;
    char *str = "this is test!";
    wchar_t *chinese_str = L"这是yi个测试啊！";

    int i = 0;
    struct font_bitmap bitmap;
    int lcd_x, lcd_y;

    if (argc != 2) {
        printf("usage %s <font_file>", argv[0]);
        return -1;
    }

    (void) dismgr_init();
    dismgr_set_default("fb");
    dismgr_init_default_display();

    (void) fontmgr_init();
    if (fontmgr_set_font("freetype", argv[1])) {
        printf("failed to set font\n");
        return -1;
    }

    fontmgr_set_font_size(30);
    lcd_x = lcd_y = 500;
    dis_buf = dismgr_get_display_buf();

    while (str[i]) {
        bitmap.origin.cur_origin_x = lcd_x;
        bitmap.origin.cur_origin_y = lcd_y;
        if (fontmgr_get_font_bitmap(str[i++], &bitmap)) {
            printf("failed to get font bitmap\n");
            return -1;
        }

        dismgr_draw_font_bitmap(&bitmap, 0x004400);
        dismgr_flush(&bitmap.region, dis_buf);

        lcd_x = bitmap.origin.next_origin_x;
        lcd_y = bitmap.origin.next_origin_y;
    }

    lcd_x = lcd_y = 100;

    i = 0;
    while (str[i]) {
        bitmap.origin.cur_origin_x = lcd_x;
        bitmap.origin.cur_origin_y = lcd_y;
        if (fontmgr_get_font_bitmap(chinese_str[i++], &bitmap)) {
            printf("failed to get font bitmap\n");
            return -1;
        }

        dismgr_draw_font_bitmap(&bitmap, 0x444400);
        dismgr_flush(&bitmap.region, dis_buf);

        lcd_x = bitmap.origin.next_origin_x;
        lcd_y = bitmap.origin.next_origin_y;
    }

    return -1;
}
