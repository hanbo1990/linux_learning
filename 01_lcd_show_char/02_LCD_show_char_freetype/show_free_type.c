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

#include "utils/error_functions.h"

static int fb_fd;
static struct fb_var_screeninfo fb_var;
static int line_width_byte, pixel_width, screen_size_byte;
static unsigned char *fb_mmap;

void lcd_put_pixel(int x, int y, unsigned int color)
{
    unsigned char *pen_8 = fb_mmap + y * line_width_byte + x * pixel_width;
    unsigned int red, green, blue;
    unsigned short *pen_16;
    unsigned int *pen_32;

    pen_16 = (unsigned short *) pen_8;
    pen_32 = (unsigned int *) pen_8;

    switch (fb_var.bits_per_pixel) {
    case 8: {
        *pen_8 = color;
        break;
    }
    case 16: {
        /* 565 */
        red = (color >> 16) & 0xff;
        green = (color >> 8) & 0xff;
        blue = (color >> 0) & 0xff;
        color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        *pen_16 = color;
        break;
    }
    case 32: {
        *pen_32 = color;
        break;
    }
    default: {
        printf("can't surport %dbpp\n", fb_var.bits_per_pixel);
        break;
    }
    }
}

void draw_bitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y)
{
    FT_Int i, j, p, q;
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;

    for (i = x, p = 0; i < x_max; i++, p++) {
        for (j = y, q = 0; j < y_max; j++, q++) {
            if (i < 0 || j < 0 || i >= fb_var.xres || j >= fb_var.yres)
                continue;

            lcd_put_pixel(i, j, bitmap->buffer[q * bitmap->width + p]);
        }
    }
}

int main(void)
{
    /* Step 1: open the LCD controller */
    fb_fd = open("/dev/fb0", O_RDWR);

    if (-1 == fb_fd) {
        errExit("opening LCD device");
    }

    /* Step 2: get the LCD configuration, resolution and BPP */
    if (-1 == ioctl(fb_fd, FBIOGET_VSCREENINFO, &fb_var)) {
        (void) close(fb_fd);
        errExit("failed getting ctl for fb0");
    }

    pixel_width = fb_var.bits_per_pixel / 8;
    line_width_byte = fb_var.xres * pixel_width;
    screen_size_byte = line_width_byte * fb_var.yres;

    /* Step 3: map the framebuffer and write data to frambuffer */
    fb_mmap = (unsigned char *) mmap(NULL, screen_size_byte, PROT_READ | PROT_WRITE, MAP_SHARED,
                                     fb_fd, 0);

    if ((unsigned char *) -1 == fb_mmap) {
        (void) close(fb_fd);
        errExit("failed to memory map to fb0");
    }

    memset(fb_mmap, 0, screen_size_byte);

    close(fb_fd);

    FT_Library library;
    FT_Error error;
    FT_Face face;
    FT_GlyphSlot slot;
    wchar_t *chinese_str = L"意";

    // prepare
    /* init library */
    error = FT_Init_FreeType(&library);
    /* error handling omitted */

    /* Create new face which loads the font file */
    error = FT_New_Face(library, "simsun.ttc", 0, &face);
    /* error handling omitted */

    /* generated bitmap will be inside the slot */
    slot = face->glyph;

    // Create bit map for the characters to show
    /* set font size */
    error = FT_Set_Pixel_Sizes(face, 50, 0);
    /* error handling omitted */

    /*
        1. Get glyph index based on the encoding
        2. Get glyph based on index
        3. Render the bitmap --> after this function, slot->bitmap contains the rendered data
    */
    error = FT_Load_Char(face, chinese_str[0], FT_LOAD_RENDER);
    /* error handling omitted */

    /* you can create bitmap which contains more complex data. */
    draw_bitmap(&slot->bitmap, fb_var.xres / 2, fb_var.yres / 2);

    munmap(fb_mmap, screen_size_byte);

    return 0;
}
