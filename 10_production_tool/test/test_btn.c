#include "display_mgr/display_mgr.h"
#include "font_8x16.c.inc"
#include "font_mgr/font_mgr.h"
#include "ui_comp_mgr/button.h"

int main(int argc, char **argv)
{
    struct button test_button;
    struct region btn_region = {
        .height = 50,
        .left_up_point =
            {
                .x_pos = 300,
                .y_pos = 300,
            },
        .width = 100,
    };
    struct display_buf *dis_buf;

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
    dis_buf = dismgr_get_display_buf();

    button_init(&test_button, "test", "LCD", &btn_region, NULL, NULL);

    test_button.on_draw(&test_button, dismgr_get_display_buf());

    return -1;
}
