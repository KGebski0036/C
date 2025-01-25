#ifndef HELPER_H
# define HELPER_H

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 5679

#define MAX_CLIENTS 10

int setupServer(int port);
void *handleClient(void *arg);
void broadcastMessage(const char *message, int sender_sock);

void *receiveMessages(void *arg);

#endif