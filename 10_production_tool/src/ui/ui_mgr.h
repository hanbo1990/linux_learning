#ifndef _UI_MGR_H
#define _UI_MGR_H

struct page {
    char* name;
    void (*run)(void* params);
    struct page* next;
};

/**
 * @brief API function to get certain page
 *
 * @param name
 * @return struct page*
 */
struct page* uimgr_get_page(char* name);

/**
 * @brief API function to init the ui manager.
 *
 * @return 0 if success and 1 if failure
 */
int uimgr_init(void);

/**
 * @brief API function to deinit the ui manager.
 *
 * @return 0 if success and 1 if failure
 */
int uimgr_deinit(void);

/**
 * @brief Provide low level child modules to register themselves to ui manager.
 *
 * @param page [in] Pointer to page
 */
void uimgr_resiter_page(struct page* page);

#endif /* _UI_MGR_H */
