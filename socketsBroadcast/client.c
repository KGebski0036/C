#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "helper.h"

int main(int argc, char **argv)
{
    char *serv_ip = "127.0.0.1";
    int server_port = DEFAULT_PORT;

    if (argc > 1)
        server_port = atoi(argv[1]);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
    serv_addr.sin_port = htons(server_port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Failed to create socket");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Failed to connect to server");
        close(sock);
        exit(1);
    }

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receiveMessages, &sock);

    char buffer[BUFFER_SIZE];
    int n;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        n = strlen(buffer);
        if (buffer[n - 1] == '\n')
            buffer[--n] = '\0';

        if (write(sock, buffer, n) < 0)
        {
            perror("Failed to write to server");
            break;
        }

        if (strcmp(buffer, "q") == 0)
        {
            printf("Exiting...\n");
            break;
        }
    }

    close(sock);
    return 0;
}

void *receiveMessages(void *arg)
{
    int sock = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int n;

    while ((n = read(sock, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("\nServer: %s\n", buffer);
    }

    printf("Server disconnected.\n");
    exit(0);
}