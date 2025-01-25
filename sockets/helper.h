#ifndef HELPER_H
# define HELPER_H

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 5679

int setupServer(int port);
void replyToClient(int sock, int client_sock);

#endif