#include "ui_mgr.h"

#include <string.h>

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

    // bug here lol
    while (tmp) {
        if (strcmp(tmp->name, name) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }

    return 0;
}

int uimgr_init(void)
{
    /* leave here for now for future extension*/

    return 0;
}

int uimgr_deinit(void)
{
    /* leave here for now for future extension*/

    return 0;
}
