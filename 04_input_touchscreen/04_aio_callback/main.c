#include <aio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int fd;
static struct aiocb aiocbp;
struct input_event event;

static void aio_complete_handler(__sigval_t sigval);

static void read_from_event(void)
{
    /* read from /dev/input/event0 of the specific input_event data*/
    bzero((char *) &aiocbp, sizeof(struct aiocb));
    aiocbp.aio_fildes = fd;
    aiocbp.aio_buf = &event;
    aiocbp.aio_offset = 0;
    aiocbp.aio_nbytes = sizeof(struct input_event);

    /* create a new thread to execute the callback function */
    aiocbp.aio_sigevent.sigev_notify = SIGEV_THREAD;
    aiocbp.aio_sigevent.sigev_notify_function = aio_complete_handler;
    aiocbp.aio_sigevent.sigev_notify_attributes = NULL;
    aiocbp.aio_sigevent.sigev_value.sival_ptr = &aiocbp;

    aio_read(&aiocbp);
}

static void aio_complete_handler(__sigval_t sigval)
{
    struct aiocb *req;
    int ret;
    struct input_event *event;
    int index;

    req = (struct aiocb *) sigval.sival_ptr;

    if (aio_error(req) == 0) {
        ret = aio_return(req);
        event = (struct input_event *) req->aio_buf;
        printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event->type, event->code, event->value);

        /* read from next */
        read_from_event();
    } else {
        printf("failed to read data\n");
    }
}

int main(int argc, char **argv)
{
    struct input_event event;

    if (argc != 2) {
        printf("Usage: %s <dev>\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        printf("failed to open file\n");
    }

    read_from_event();

    while (1) { /* go and do something in this thread */
    }

    return 0;
}