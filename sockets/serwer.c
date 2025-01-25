#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include "helper.h"

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    if (argc > 1)
        port = atoi(argv[1]);

    int sock = setupServer(port);
    struct sockaddr_in cl_addr;

    while (1)
    {
        socklen_t clen = sizeof(cl_addr);
        int client_sock = accept(sock, (struct sockaddr *)&cl_addr, &clen);
        if (client_sock < 0)
        {
            perror("Failed to accept connection");
        }
        else
        {
            printf("New client\n");
            replyToClient(sock, client_sock);
        }
    }

    return 0;
}

int setupServer(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Failed to create socket");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Failed to bind socket");
        close(sock);
        exit(1);
    }

    listen(sock, 5);
    printf("Listening on port %d\n", port);

    return sock;
}

void replyToClient(int sock, int client_sock)
{
    if (fork() == 0)
    {
        close(sock);
        char buffer[BUFFER_SIZE];
        int n;

        while ((n = read(client_sock, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[n] = '\0';
            printf("Received: %s\n", buffer);

            if (buffer[0] == 'q')
            {
                printf("Close conection: %s\n", buffer);
                write(client_sock, "\0", 1);
                shutdown(sock, SHUT_RDWR);
                break;
            }

            write(client_sock, buffer, n);
        }

        close(client_sock);
        exit(0);
    }
    else
    {
        close(client_sock);
    }
}
