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

    (void) dismgr_init();
    dismgr_set_default("fb");
    dismgr_init_default_display();
    (void) fontmgr_init();
    if (fontmgr_set_font("freetype", "simsun.ttc")) {
        printf("failed to set font\n");
        return -1;
    }

    button_init(&test_button, "test", "example", &btn_region, NULL, NULL);

    test_button.on_draw(&test_button, dismgr_get_display_buf());

    return 0;
}
