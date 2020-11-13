#include <fcntl.h>
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

/* below defined in lib\fonts\font_8x16.c */
#include "font_8x16.c.inc"

static int fb_fd, cn_char_fd;
static struct fb_var_screeninfo fb_var;
static int line_width_byte, pixel_width, screen_size_byte, cn_char_file_size;
static unsigned char *fb_mmap;
static unsigned char *cn_char_mmap;

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

void lcd_put_ascii(int x, int y, unsigned char c, unsigned int color)
{
    unsigned char *dots = (unsigned char *) &fontdata_8x16[c * 16];
    int i, b;
    unsigned char byte;

    for (i = 0; i < 16; i++) {
        byte = dots[i];
        for (b = 7; b >= 0; b--) {
            if (byte & (1 << b)) {
                /* show */
                lcd_put_pixel(x + 7 - b, y + i, color);
            } else {
                /* hide */
                lcd_put_pixel(x + 7 - b, y + i, 0);
            }
        }
    }
}

void lcd_put_chinese_char(int x, int y, unsigned char *chinese_chr, int color)
{
    /* 16*16 pixels for each chinese char, that is 32 bytes
     HZK16 contains multiple zones, each zone has 94 chars
     one char consists of MSB LSB
     MSB is area code, actual area of this char is MSB - 0xA1 (to align with ASCII)
     LSB is offset in area, the offset is LSB - 0xA1 */

    unsigned int area = chinese_chr[0] - 0xA1;
    unsigned int offset = chinese_chr[1] - 0xA1;
    unsigned char *dots = cn_char_mmap + (area * 94 + offset) * 32;
    unsigned char byte;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 2; j++) {
            byte = dots[i * 2 + j];
            for (int k = 0; k < 8; k++) {
                if (byte & (1 << k)) {
                    lcd_put_pixel(x + j * 8 + 7 - k, y + i, color);
                } else {
                    lcd_put_pixel(x + j * 8 + 7 - k, y + i, 0);
                }
            }
        }
    }
}

void lcd_put_str(int x, int y, unsigned char *str, unsigned int color)
{
    int line_nr = y;
    int colum_nr = x;
    for (int i = 0; i < strlen(str); i++) {
        if ('\n' != str[i]) {
            lcd_put_ascii(colum_nr, line_nr, str[i], color);
            colum_nr += 8;
        } else {
            line_nr += 16;
            colum_nr = x;
        }
    }
}

void open_cn_char_file(void)
{
    struct stat file_stat;

    cn_char_fd = open("HZK16", O_RDONLY);
    if (-1 == cn_char_fd) {
        errExit("failed to open cn char file");
    }

    if (-1 == fstat(cn_char_fd, &file_stat)) {
        close(cn_char_fd);
        errExit("failed to stat cn char file");
    }

    cn_char_file_size = file_stat.st_size;
    cn_char_mmap =
        (unsigned char *) mmap(NULL, cn_char_file_size, PROT_READ, MAP_SHARED, cn_char_fd, 0);

    if ((unsigned char *) -1 == cn_char_mmap) {
        close(cn_char_fd);
        errExit("failed to map cn char file");
    }

    close(cn_char_fd);
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

    /* Step 4: Play with the framebuffer */
    lcd_put_ascii(fb_var.xres / 2, fb_var.yres / 2, 'A', 0x007D7D00);
    lcd_put_ascii(fb_var.xres / 2 + 8, fb_var.yres / 2, 'B', 0x007D7D00);
    lcd_put_str(fb_var.xres / 2, fb_var.yres / 2 + 16, "This is a \ntest\nString", 0x00FF00FF);

    open_cn_char_file();

    // -fexec-charset=GB2312 must be passed when using cn char
    unsigned char str[] = "中";
    lcd_put_chinese_char(fb_var.xres / 8, fb_var.yres / 8, str, 0x007D00FF);

    munmap(fb_mmap, screen_size_byte);
    munmap(cn_char_mmap, cn_char_file_size);

    return 0;
}
