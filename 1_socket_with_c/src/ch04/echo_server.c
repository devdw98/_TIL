#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../lib/err.h"

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    int server_sock, client_sock;
    char message[BUFF_SIZE];
    int str_len, i;

    struct sockaddr_in server_addr, client_addr; // arpa/inet.h
    socklen_t client_addr_size;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof(server_addr)); // string.h
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        error_handling("bind() error");

    if (listen(server_sock, 5) == -1)
        error_handling("listen() error");

    client_addr_size = sizeof(client_addr);
    for (i = 0; i < 5; i++)
    {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
        if (client_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d\n", i + 1);

        while ((str_len = read(client_sock, message, BUFF_SIZE)) != 0)
        {
            write(client_sock, message, str_len);
            close(client_sock);
        }
    }
    close(server_sock);
    return 0;
}