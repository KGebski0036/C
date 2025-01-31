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

int client_count = 0;
int udp_sock;
struct sockaddr_in udp_client_addrs[MAX_CLIENTS];
int udp_client_count = 0;
int client_sockets[MAX_CLIENTS];
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handleUDPClient(void *arg);
void *handleClient(void *arg);
void broadcastMessage(const char *message, int sender_sock);
int setupServer(int port);

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    if (argc > 1)
        port = atoi(argv[1]);

    int tcp_sock = setupServer(port);
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    bind(udp_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    pthread_t udp_thread;
    pthread_create(&udp_thread, NULL, handleUDPClient, &udp_sock);
    pthread_detach(udp_thread);

    struct sockaddr_in cl_addr;
    while (1)
    {
        socklen_t clen = sizeof(cl_addr);
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(tcp_sock, (struct sockaddr *)&cl_addr, &clen);
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
            close(*client_sock);
            free(client_sock);
        }
        pthread_mutex_unlock(&client_mutex);
    }
    return 0;
}

void *handleUDPClient(void *arg)
{
    int udp_sock = *(int *)arg;  // Extract UDP socket descriptor
    char buffer[BUFFER_SIZE];
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    while (1)
    {
        int n = recvfrom(udp_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cli_addr, &cli_len);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("UDP Client: %s\n", buffer);
            
            pthread_mutex_lock(&client_mutex);

            // Ensure the client address is stored for future broadcasts
            int known_client = 0;
            for (int i = 0; i < udp_client_count; i++)
            {
                if (memcmp(&udp_client_addrs[i], &cli_addr, sizeof(cli_addr)) == 0)
                {
                    known_client = 1;
                    break;
                }
            }
            if (!known_client && udp_client_count < MAX_CLIENTS)
            {
                udp_client_addrs[udp_client_count++] = cli_addr;
            }
            pthread_mutex_unlock(&client_mutex);

            // Broadcast to both TCP and UDP clients
            broadcastMessage(buffer, -1);
        }
    }
    return NULL;
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
    
    // Broadcast message to all UDP clients
    for (int i = 0; i < udp_client_count; i++)
    {
        sendto(udp_sock, message, strlen(message), 0, (struct sockaddr *)&udp_client_addrs[i], sizeof(udp_client_addrs[i]));
    }
    pthread_mutex_unlock(&client_mutex);
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

    listen(sock, MAX_CLIENTS);
    printf("Listening on port %d\n", port);

    return sock;
}