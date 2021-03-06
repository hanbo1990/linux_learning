#include "socket_input.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "input_mgr/input_mgr.h"
#include "utils/custom_defines.h"

#define SERVER_PORT 2222

int fd;
socklen_t addr_len = sizeof(struct sockaddr);

static int get_event(struct input_event* input_event)
{
    int rec_len;
    struct sockaddr_in client_address;
    struct report_event_data* ts_data;

    input_event->data = (void*) calloc(1, sizeof(struct report_event_data));
    if (NULL == input_event->data) {
        LOG_ERROR("Malloc failure");
        return -1;
    }

    ts_data = (struct report_event_data*) input_event->data;
    DEFAULT_REPORT_DATA(ts_data);

    rec_len = recvfrom(fd, ts_data->str, MAX_REPORT_DATA_STR_LEN, 0,
                       (struct sockaddr*) &client_address, &addr_len);
    if (rec_len <= 0 || rec_len > MAX_REPORT_DATA_STR_LEN) {
        LOG_WARN("invalid data received!");
        custom_free_and_set_null(&input_event->data);
        return -1;
    }

    ts_data->str[rec_len] = '\0';
    LOG_INFO("get message from %s: %s\n", inet_ntoa(client_address.sin_addr), ts_data->str);

    input_event->type = EVENT_TYPE__STR_REPORT;
    gettimeofday(&input_event->time, NULL);

    return 0;
}

static int dev_init(void)
{
    struct sockaddr_in server_address;
    int ret;

    /* open socket type SOCK_DGRAM (UDP)*/
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        LOG_ERROR("failed creating socket\n");
        return -1;
    }

    /* bind ip and port */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    memset(server_address.sin_zero, 0, 8);

    ret = bind(fd, (const struct sockaddr*) &server_address, addr_len);
    if (ret) {
        LOG_ERROR("Failed binding socket\n");
        return -1;
    }

    return 0;
}

static int dev_deinit(void)
{
    close(fd);
    return 0;
}

static struct input_device device = {
    .name = "socketinput",
    .get_event = get_event,
    .dev_init = dev_init,
    .dev_deinit = dev_deinit,
};

int socket_input_init(void)
{
    return inputmgr_resigter(&device);
}
