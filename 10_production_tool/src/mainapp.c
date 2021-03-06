#include "display_mgr/display_mgr.h"
#include "font_mgr/font_mgr.h"
#include "input_mgr/input_mgr.h"
#include "ui/ui_mgr.h"

int main(int argc, char** argv)
{
    dismgr_init();
    dismgr_set_default("fb");
    dismgr_init_default_display();

    inputmgr_init();

    fontmgr_init();
    if (fontmgr_set_font("freetype", "simsun.ttc")) {
        printf("failed to set font\n");
        return -1;
    }

    uimgr_init();

    uimgr_get_page("main")->run(NULL);

    return 0;
}
