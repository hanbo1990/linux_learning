#ifndef _FONT_MGR_H
#define _FONT_MGR_H

#include "utils/custom_defines.h"

struct font_opt{
    char* name;
    int (*font_init)(char *filename);
    int (*set_font_size)(int font_size);
    int (*get_font_bitmap)(unsigned int code, struct font_bitmap* bitmap);
    struct font_opt *next;
};

int fontmgr_init(void);
int fontmgr_register(struct font_opt* opt);
int fontmgr_set_font_size(int font_size);
int fontmgr_set_font(char* font_opt_name, char* font_file);
int fontmgr_get_font_bitmap(unsigned int code, struct font_bitmap* bitmap);

#endif /* _FONT_MGR_H */
