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

void *receiveUDPMessages(void *arg);

int main(int argc, char **argv)
{
    char *serv_ip = "127.0.0.1";
    int server_port = DEFAULT_PORT;
    
    if (argc > 1) server_port = atoi(argv[1]);

    struct sockaddr_in serv_addr, client_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
    serv_addr.sin_port = 0;
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Bind to allow receiving broadcast messages
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(server_port);
    bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr));
    
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receiveUDPMessages, &sock);
    pthread_detach(recv_thread);
    
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        int n = strlen(buffer);
        if (buffer[n - 1] == '\n') buffer[--n] = '\0';
        sendto(sock, buffer, n, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (strcmp(buffer, "q") == 0) break;
    }
    close(sock);
    return 0;
}

void *receiveUDPMessages(void *arg)
{
    int sock = *(int *)arg;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in recv_addr;
    socklen_t recv_len = sizeof(recv_addr);
    
    while (1)
    {
        int n = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recv_addr, &recv_len);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("\nMessage: %s\n", buffer);
        }
    }
    return NULL;
}