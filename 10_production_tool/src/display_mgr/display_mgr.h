#ifndef _DISPLAY_MGR_H
#define _DISPLAY_MGR_H

#include "utils/custom_defines.h"

struct display_buf {
    int x_res; /* pixel */
    int y_res; /* pixel */
    int bpp;   /* bits per pixel */
    unsigned char *buf;
};

struct display_opr {
    char *name;

    int (*dev_init)(void);
    int (*dev_deinit)(void);
    int (*get_buf)(struct display_buf *dis_buf);
    int (*flush_region)(struct region *region, struct display_buf *dis_buf);
    struct display_opr *next;
};

int dismgr_init(void);
int dismgr_deinit(void);
int dismgr_register_display(struct display_opr *new_dis_op);
int dismgr_set_default(char *name);
int dismgr_init_default_display(void);
int dismgr_draw_pixel(int x, int y, unsigned int color);
void dismgr_draw_font_bitmap(struct font_bitmap *bitmap, int color);
void dismgr_draw_region(struct region *region, int color);
void dismgr_draw_text_in_region(char *text, struct region *region, int color);
int dismgr_flush(struct region *region, struct display_buf *dis_buf);
struct display_buf *dismgr_get_display_buf(void);

#endif /* _DISPLAY_MGR_H */