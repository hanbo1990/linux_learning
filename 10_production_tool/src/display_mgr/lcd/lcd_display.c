#include "lcd_display.h"

#include <fcntl.h>
#include <linux/fb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "display_mgr/display_mgr.h"
#include "utils/custom_defines.h"

static bool init_status = false;
static int fb_fd;
static unsigned char *fb_mmap;
static struct fb_var_screeninfo fb_var;

static int line_width_byte, pixel_width, screen_size_byte;

static int device_init(void)
{
    /* Step 1: open the LCD controller */
    fb_fd = open("/dev/fb0", O_RDWR);
    if (-1 == fb_fd) {
        LOG_ERROR("failed opening LCD device");
        return -1;
    }

    /* Step 2: get the LCD configuration, resolution and BPP */
    if (-1 == ioctl(fb_fd, FBIOGET_VSCREENINFO, &fb_var)) {
        (void) close(fb_fd);
        LOG_ERROR("failed getting ctl for fb0");
        return -1;
    }

    pixel_width = fb_var.bits_per_pixel / 8;
    line_width_byte = fb_var.xres * pixel_width;
    screen_size_byte = line_width_byte * fb_var.yres;

    /* Step 3: map the framebuffer and write data to frambuffer */
    fb_mmap = (unsigned char *) mmap(NULL, screen_size_byte, PROT_READ | PROT_WRITE, MAP_SHARED,
                                     fb_fd, 0);
    if ((unsigned char *) -1 == fb_mmap) {
        (void) close(fb_fd);
        LOG_ERROR("failed to memory map to fb0");
        return -1;
    }

    init_status = true;

    return 0;
}

static int device_deinit(void)
{
    RETURN_IF_NOT_SET_WITH_LOG(init_status, "device not inited");

    close(fb_fd);
    munmap(fb_mmap, screen_size_byte);
    init_status = false;
    return 0;
}

/* if we return the framebuffer used by lcd, it means the upper
 * application is directly wirting to LCD, then the flush_region
 * can be empty.
 */
int get_buf(struct display_buf *dis_buf)
{
    RETURN_IF_NOT_SET_WITH_LOG(init_status, "device not inited");

    dis_buf->x_res = fb_var.xres;
    dis_buf->y_res = fb_var.yres;
    dis_buf->bpp = fb_var.bits_per_pixel;
    dis_buf->buf = fb_mmap;
    return 0;
}
static int flush_region(struct region *region, struct display_buf *dis_buf)
{
    RETURN_IF_NOT_SET_WITH_LOG(init_status, "device not inited");

    return 1;
};

static struct display_opr display_operation = {
    .name = "fb",
    .dev_init = device_init,
    .dev_deinit = device_deinit,
    .get_buf = get_buf,
    .flush_region = flush_region,
};

int lcd_init(void)
{
    return dismgr_register_display(&display_operation);
}