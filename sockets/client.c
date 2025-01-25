#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
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

        n = read(sock, buffer, sizeof(buffer) - 1);
        if (n < 0)
        {
            perror("Coudn't read from server\n");
            break;
        }
        else if (n == 0)
        {
            perror("Connection ended.\n");
            shutdown(sock, SHUT_RDWR);
            break;
        }

        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "q") == 0)
        {
            shutdown(sock, SHUT_WR);
        }
    }

    close(sock);
    return 0;
}
