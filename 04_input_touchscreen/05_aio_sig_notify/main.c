#include <aio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int fd;
static struct aiocb aiocbp;
struct input_event event;

static void aio_complete_handler(int signo, siginfo_t *info, void *context);

static void read_from_event(void)
{
    static bool aio_struct_ready = false;

    if (!aio_struct_ready) {
        /* read from /dev/input/event0 of the specific input_event data*/
        bzero((char *) &aiocbp, sizeof(struct aiocb));
        aiocbp.aio_fildes = fd;
        aiocbp.aio_buf = &event;
        aiocbp.aio_offset = 0;
        aiocbp.aio_nbytes = sizeof(struct input_event);

        /* application emit SIGIO when file is readable */
        aiocbp.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        aiocbp.aio_sigevent.sigev_signo = SIGIO;
        aiocbp.aio_sigevent.sigev_value.sival_ptr = &aiocbp;
    }

    aio_read(&aiocbp);
}

static void aio_complete_handler(int signo, siginfo_t *info, void *context)
{
    struct aiocb *req;
    struct input_event *event;

    /* Ensure it's our signal */
    if (info->si_signo == SIGIO) {
        req = (struct aiocb *) info->si_value.sival_ptr;

        /* Did the request complete? */
        if (aio_error(req) == 0) {
            /* Request completed successfully, get the return status */
            aio_return(req);
            event = (struct input_event *) req->aio_buf;
            printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event->type, event->code, event->value);
            read_from_event();
        }
    }
}

int main(int argc, char **argv)
{
    struct input_event event;
    sigset_t sigset;
    struct sigaction sig_act;

    if (argc != 2) {
        printf("Usage: %s <dev>\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        printf("failed to open file\n");
    }

    /* Prepare the sig action */
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = SA_SIGINFO; /*The signal handler takes three arguments, not one.*/
    sig_act.sa_sigaction = aio_complete_handler;

    /* Link the sigaction to listen to SIGIO */
    sigaction(SIGIO, &sig_act, NULL);

    read_from_event();

    while (1) { /* go and do something in this thread */
    }

    return 0;
}