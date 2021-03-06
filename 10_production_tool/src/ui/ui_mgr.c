#include "ui_mgr.h"

#include <string.h>

#include "main_page.h"
#include "utils/custom_defines.h"

static struct page* pages = NULL;

void uimgr_resiter_page(struct page* page)
{
    page->next = pages;
    pages = page;
}

struct page* uimgr_get_page(char* name)
{
    struct page* tmp = pages;

    while (tmp) {
        if (strcmp(tmp->name, name) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }

    return 0;
}

void uimgr_init(void)
{
    mainpage_init();
}
