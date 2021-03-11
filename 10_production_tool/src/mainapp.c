#include "display_mgr/display_mgr.h"
#include "font_mgr/font_mgr.h"
#include "input_mgr/input_mgr.h"
#include "ui/ui_mgr.h"
#include "utils/modules.h"

extern init_fn_t START_FUNC_LEVEL(0);
extern init_fn_t STOP_FUN_LEVEL(0);

int child_module_init(void)
{
    const init_fn_t* fn_ptr;

    for (fn_ptr = &START_FUNC_LEVEL(0); fn_ptr < &STOP_FUN_LEVEL(0); fn_ptr++) {
        if ((*fn_ptr)()) {
            fprintf(stderr, "failed to init function at %p\n", fn_ptr);
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    /* Init all manager modules */
    dismgr_init();
    inputmgr_init();
    fontmgr_init();
    uimgr_init();

    /* init child modules attached to managers */
    child_module_init();

    /* Seeting up and configure the manager modules */
    dismgr_set_default("fb");
    dismgr_init_default_display();

    inputmgr_init_all_devices();

    if (fontmgr_set_font("freetype", "simsun.ttc")) {
        printf("failed to set font\n");
        return -1;
    }

    /* start the main loop here */
    uimgr_get_page("main")->run(NULL);

    return 0;
}
