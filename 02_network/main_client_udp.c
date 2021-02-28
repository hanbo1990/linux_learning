#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
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
    unsigned char sendbuf[1000] = "";
    int send_len;
    int ret;
    socklen_t addr_len = sizeof(struct sockaddr);

    if (2 != argc) {
        printf("usage %s ip_address\n", argv[0]);
        return -1;
    }

    /* open socket type SOCK_DGRAM*/
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        printf("failed to open socket\n");
        return -1;
    }

    /* connect to server */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    if (0 == inet_aton(argv[1], &server_address.sin_addr)) {
        printf("invalid server ip address");
        close(fd);
        return -1;
    }
    memset(server_address.sin_zero, 0, 8);

#ifdef METHOD_CONNECT
    ret = connect(fd, (const struct sockaddr*) &server_address, addr_len);
    if (ret) {
        printf("failed to connect socket\n");
    }
#endif

    while (1) {
        if (fgets(sendbuf, 999, stdin)) {
#ifdef METHOD_CONNECT
            send_len = send(fd, sendbuf, strlen(sendbuf), 0);
#else
            send_len = sendto(fd, sendbuf, strlen(sendbuf), 0,
                              (const struct sockaddr*) &server_address, addr_len);
#endif
            if (send_len <= 0) {
                printf("failed to send\n");
                close(fd);
                return -1;
            } else {
                printf("data sent %s\n", sendbuf);
            }
        }
    }
}