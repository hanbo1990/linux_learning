#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

/* get input info /dev/input/event0 */
int main(int argc, char** argv)
{
    int fd;
    int err;
    int len;
    int i;
    int bit;
    unsigned char byte;
    struct input_id id;
    unsigned int evbit[2];
    char* evnames[] = {
        [0x00] = "EV_SYN", [0x01] = "EV_KEY",  [0x02] = "EV_REL", [0x03] = "EV_ABS",
        [0x04] = "EV_MSC", [0x05] = "EV_SW	", [0x11] = "EV_LED", [0x12] = "EV_SND",
        [0x14] = "EV_REP", [0x15] = "EV_FF	", [0x16] = "EV_PWR", [0x17] = "EV_FF_STATUS",
    };

    if (2 != argc) {
        printf("Usage: %s <dev>\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        printf("open %s error\n", argv[1]);
        return -1;
    }

    err = ioctl(fd, EVIOCGID, &id);
    if (0 == err) {
        printf("bustype 0x%x\n", id.bustype);
        printf("vendor 0x%x\n", id.vendor);
        printf("product 0x%x\n", id.product);
        printf("version 0x%x\n", id.version);
    }

    /* you need to read evdev.c to know more about what to read*/
    len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
    if (len > 0 && len < sizeof(evbit)) {
        for (i = 0; i < len; i++) {
            byte = ((unsigned char*) evbit)[i];
            for (bit = 0; bit < 8; bit++) {
                if (byte & (1 << bit)) {
                    printf("%s is set\n", evnames[i * 8 + bit]);
                }
            }
        }
    }

    return 0;
}