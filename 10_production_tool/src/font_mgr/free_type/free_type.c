#include "free_type.h"

#include <fcntl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <linux/fb.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "font_mgr/font_mgr.h"
#include "utils/custom_defines.h"

FT_Face face;

static int font_init(char* filename)
{
    FT_Library library;
    FT_Error error;

    // prepare
    /* init library */
    error = FT_Init_FreeType(&library);
    if (error) {
        LOG_ERROR("Failed to init freetype");
        return -1;
    }

    /* Create new face which loads the font file */
    error = FT_New_Face(library, filename, 0, &face);
    if (error) {
        LOG_ERROR("Failed to load font file");
        return -1;
    }

    return 0;
}
static int set_font_size(int font_size)
{
    (void) FT_Set_Pixel_Sizes(face, font_size, 0);
    /* free type can set all size */
    return 0;
}
static int get_font_bitmap(unsigned int code, struct font_bitmap* bitmap)
{
    FT_Vector pen;
    FT_Error error;
    FT_GlyphSlot slot = face->glyph;

    pen.x = bitmap->origin.cur_origin_x * 64; /* 1 x is 1/64 pixel */
    pen.y = bitmap->origin.cur_origin_y * 64;

    FT_Set_Transform(face, 0, &pen);
    /* load glyph image into the slot, erase previous one */
    error = FT_Load_Char(face, code, FT_LOAD_RENDER);
    if (error) {
        LOG_ERROR("load char failed!");
        return -1;
    }

    bitmap->bitmap_buf = slot->bitmap.buffer;

    bitmap->region.left_up_point.x_pos = slot->bitmap_left;
    bitmap->region.left_up_point.y_pos = bitmap->origin.cur_origin_y * 2 - slot->bitmap_top;
    bitmap->region.width = slot->bitmap.width;
    bitmap->region.height = slot->bitmap.rows;

    bitmap->origin.next_origin_x = bitmap->origin.cur_origin_x + slot->advance.x / 64;
    bitmap->origin.next_origin_y = bitmap->origin.cur_origin_y;
    return 0;
}

static struct font_opt freetype_font = {
    .name = "freetype",
    .font_init = font_init,
    .set_font_size = set_font_size,
    .get_font_bitmap = get_font_bitmap,
};

int freetype_init(void)
{
    return fontmgr_register(&freetype_font);
}
