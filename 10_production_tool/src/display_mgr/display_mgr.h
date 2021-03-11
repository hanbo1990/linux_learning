#ifndef _DISPLAY_MGR_H
#define _DISPLAY_MGR_H

#include "utils/custom_defines.h"

/**
 * @brief Display buffer contains all display-realted information
 *
 */
struct display_buf {
    int x_res;          /*< pixel */
    int y_res;          /*< pixel */
    int bpp;            /*< bits per pixel */
    unsigned char *buf; /*< pointer to buffer */
};

/**
 * @brief display_opr defines behavior of low level child
 *
 */
struct display_opr {
    char *name;
    int (*dev_init)(void);
    int (*dev_deinit)(void);
    int (*get_buf)(struct display_buf *dis_buf);
    int (*flush_region)(struct region *region, struct display_buf *dis_buf);
    struct display_opr *next;
};

/**
 * @brief API function to init display manager
 *
 * @return int 0 if success and 1 if failure
 */
int dismgr_init(void);

/**
 * @brief API function to deinit display manager
 *
 * @return int 0 if success and 1 if failure
 */
int dismgr_deinit(void);

/**
 * @brief API function to set default display
 *
 * @param name [in] name of the display
 * @return int 0 if success and 1 if failure
 */
int dismgr_set_default(char *name);

/**
 * @brief API function to init default display
 *
 * @return int 0 if success and 1 if failure
 */
int dismgr_init_default_display(void);

/**
 * @brief API function to draw one pixel
 *
 * @param x [in] x position
 * @param y [in] y position
 * @param color [in] Color
 * @return int 0 if success and 1 if failure
 */
int dismgr_draw_pixel(int x, int y, unsigned int color);

/**
 * @brief API function to draw a font bitmap
 *
 * @param bitmap [in] Pointer to font bitmap
 * @param color [in]
 */
void dismgr_draw_font_bitmap(struct font_bitmap *bitmap, int color);

/**
 * @brief API function to fill a region with color
 *
 * @param region [in] Pointer to region
 * @param color [in]
 */
void dismgr_draw_region(struct region *region, int color);

/**
 * @brief API function to draw text in given region, all text will be middle aligned.
 *
 * @param text [in]
 * @param region [in]
 * @param color [in]
 */
void dismgr_draw_text_in_region(char *text, struct region *region, int color);

/**
 * @brief API function to flush the display.
 *
 * @param region [in] Region to flush
 * @param dis_buf [in] Pointer to display buffer
 * @return int 0 if success and 1 if failure
 */
int dismgr_flush(struct region *region, struct display_buf *dis_buf);

/**
 * @brief API function to get display buffer
 *
 * @return struct display_buf*
 */
struct display_buf *dismgr_get_display_buf(void);

/**
 * @brief Provide low level child modules to register themselves.
 *
 * @param new_dis_op [in] Pointer to display_opr
 * @return 0 if success and 1 if failure
 */
int dismgr_register(struct display_opr *new_dis_op);

#endif /* _DISPLAY_MGR_H */