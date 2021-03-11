#ifndef _FONT_MGR_H
#define _FONT_MGR_H

#include "utils/custom_defines.h"

/**
 * @brief font_opt defines behavior of low level child
 *
 */
struct font_opt{
    char* name;
    int (*font_init)(char *filename);
    int (*set_font_size)(int font_size);
    int (*get_font_bitmap)(unsigned int code, struct font_bitmap* bitmap);
    struct font_opt *next;
};

/**
 * @brief API function to init font manager
 * 
 * @return 0 if success and 1 if failure
 */
int fontmgr_init(void);

/**
 * @brief API function to deinit font manager
 * 
 * @return 0 if success and 1 if failure
 */
int fontmgr_deinit(void);

/**
 * @brief Change font size. Some font type supports all size but some not.
 * 
 * @param font_size [in] Font size in Pixels.
 * @return 0 if success and 1 if failure
 */
int fontmgr_set_font_size(int font_size);

/**
 * @brief Set font to use. This functin will load font file and init font.
 * 
 * @param font_opt_name [in] Name to the font operation
 * @param font_file [in] Font file for this font operation
 * @return 0 if success and 1 if failure
 */
int fontmgr_set_font(char* font_opt_name, char* font_file);

/**
 * @brief Get font bitmap by given font code.
 * 
 * @param code [in] Font code
 * @param bitmap [out] Pointer to the font bitmap
 * @return 0 if success and 1 if failure 
 */
int fontmgr_get_font_bitmap(unsigned int code, struct font_bitmap* bitmap);

/**
 * @brief Provide low level child modules to register themselves.
 * 
 * @param opt [in] Pointer to font_opt framwork
 * @return 0 if success and 1 if failure
 */
int fontmgr_register(struct font_opt* opt);

#endif /* _FONT_MGR_H */
