#include "font_mgr.h"

#include "free_type/free_type.h"
#include "utils/custom_defines.h"

static struct font_opt* font_ctl = NULL;
static struct font_opt* default_font_ctl = NULL;

int fontmgr_register(struct font_opt* opt)
{
    RETURN_IF_NOT_SET_WITH_LOG(opt, "invalid input");

    opt->next = font_ctl;
    font_ctl = opt;

    return 0;
}

int fontmgr_init(void)
{
    return freetype_init();
}

int fontmgr_set_font(char* font_opt_name, char* font_file)
{
    struct font_opt* tmp = font_ctl;

    while (tmp) {
        if (strcmp(tmp->name, font_opt_name) == 0) {
            break;
        }
        tmp = tmp->next;
    }

    if (NULL == tmp) {
        return -1;
    }

    default_font_ctl = tmp;
    return default_font_ctl->font_init(font_file);
}

int fontmgr_set_font_size(int font_size)
{
    if (NULL != default_font_ctl) {
        return default_font_ctl->set_font_size(font_size);
    }
    return -1;
}

int fontmgr_get_font_bitmap(unsigned int code, struct font_bitmap* bitmap)
{
    if (NULL != default_font_ctl) {
        return default_font_ctl->get_font_bitmap(code, bitmap);
    }
    return -1;
}