#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void printError(const char *err){
    fprintf(stderr, "%s\n", err);
    exit(1);
}

int main(void) {

    int fd;

    if ((fd = open("/tmp/fifotest", O_WRONLY)) < 0)
        printError("Nie mona otworzyc FIFO");

    int limit = 0;

    for(;;){
        int x = write(fd, "a", 1);
        limit += x;
        printf("limit bufforra to: %d bajt\n", limit);
    }
    close(fd);
    unlink("/tmp/fifotest");
    
}