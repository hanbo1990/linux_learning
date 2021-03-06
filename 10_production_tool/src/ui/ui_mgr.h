#ifndef _UI_MGR_H
#define _UI_MGR_H

struct page{
    char* name;
    void (*run)(void* params);
    struct page *next;
};

void uimgr_resiter_page(struct page* page);

struct page* uimgr_get_page(char* name);

void uimgr_init(void);

#endif /* _UI_MGR_H */
