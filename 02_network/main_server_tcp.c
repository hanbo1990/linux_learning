#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 1234
#define BACKLOG     15

int main(int argc, char **argv)
{
    int fd;
    int fd_client;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    unsigned char recbuf[1000] = "";
    int rec_len;
    int ret;
    socklen_t addr_len = sizeof(struct sockaddr);

    /* open socket type SOCK_STREAM*/
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd) {
        printf("failed creating socket\n");
        return -1;
    }

    /* bind ip and port */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    memset(server_address.sin_zero, 0, 8);

    ret = bind(fd, (const struct sockaddr *) &server_address, addr_len);
    if (ret) {
        printf("Failed binding socket\n");
        return -1;
    }

    /* start listen */
    ret = listen(fd, BACKLOG);
    if (ret) {
        printf("listen failed\n");
        return -1;
    }

    while (1) {
        /* accept connection */
        fd_client = accept(fd, (struct sockaddr *) &client_address, &addr_len);
        if (-1 != fd_client) {
            printf("client on address %s connected\n", inet_ntoa(client_address.sin_addr));
            if (!fork()) {
                printf("start to receive data\n");
                while (1) {
                    /* send/receive data*/
                    rec_len = recv(fd_client, recbuf, 999, 0);
                    if (rec_len <= 0) {
                        printf("receive failed\n");
                        close(fd);
                        return -1;
                    } else {
                        recbuf[rec_len] = '\0';
                        printf("received data %s\n", recbuf);
                    }
                }
            }
        }
    }

    return 0;
}