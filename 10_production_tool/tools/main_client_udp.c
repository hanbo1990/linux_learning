#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 2222

int main(int argc, char** argv)
{
    int fd;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int send_len;
    int port;
    int ret;
    char *data, *ipaddr;
    socklen_t addr_len = sizeof(struct sockaddr);

    if (4 != argc) {
        printf("usage %s <ip_address> <port> <data>\n", argv[0]);
        return -1;
    }

    ipaddr = argv[1];
    port = (int) strtol(argv[2], NULL, 10);
    data = argv[3];

    printf("sending %s to %s via port %d\n", data, ipaddr, port);

    /* open socket type SOCK_DGRAM*/
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        printf("failed to open socket\n");
        return -1;
    }

    /* connect to server */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (0 == inet_aton(ipaddr, &server_address.sin_addr)) {
        printf("invalid server ip address");
        close(fd);
        return -1;
    }
    memset(server_address.sin_zero, 0, 8);

    if (sendto(fd, data, strlen(data), 0, (const struct sockaddr*) &server_address, addr_len) <=
        0) {
        printf("failed to send\n");
        close(fd);
        return -1;
    }
}