#ifndef SHARED_H
#define SHARED_H

#define KEY 98867
#define NUMBER_OF_MESSAGES 50

void printError(const char *err)
{
    fprintf(stderr, "%s\n", err);
    exit(1);
}

typedef struct
{
    int i;
    char buf[50];
} Message;

#endif