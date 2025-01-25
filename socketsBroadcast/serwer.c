#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "helper.h"

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    if (argc > 1)
        port = atoi(argv[1]);

    int server_sock = setupServer(port);
    struct sockaddr_in cl_addr;

    printf("Server is running...\n");

    while (1)
    {
        socklen_t clen = sizeof(cl_addr);
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, (struct sockaddr *)&cl_addr, &clen);
        if (*client_sock < 0)
        {
            perror("Failed to accept connection");
            free(client_sock);
            continue;
        }

        pthread_mutex_lock(&client_mutex);
        if (client_count < MAX_CLIENTS)
        {
            client_sockets[client_count++] = *client_sock;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handleClient, client_sock);
            pthread_detach(thread_id);
        }
        else
        {
            printf("Max clients reached. Connection refused.\n");
            close(*client_sock);
            free(client_sock);
        }
        pthread_mutex_unlock(&client_mutex);
    }

    return 0;
}

void broadcastMessage(const char *message, int sender_sock)
{
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++)
    {
        if (client_sockets[i] != sender_sock)
        {
            write(client_sockets[i], message, strlen(message));
        }
    }
    pthread_mutex_unlock(&client_mutex);
}

void *handleClient(void *arg)
{
    int client_sock = *(int *)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    int n;

    while ((n = read(client_sock, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        if (strcmp(buffer, "q") == 0)
        {
            printf("Client disconnected\n");
            break;
        }

        broadcastMessage(buffer, client_sock);
    }

    close(client_sock);

    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++)
    {
        if (client_sockets[i] == client_sock)
        {
            client_sockets[i] = client_sockets[--client_count];
            break;
        }
    }
    pthread_mutex_unlock(&client_mutex);

    return NULL;
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