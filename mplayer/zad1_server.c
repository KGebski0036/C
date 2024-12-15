#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void printError(const char *err){
    fprintf(stderr, "%s\n", err);
    exit(1);
}

int main(void) {

    int fd;

    if (mkfifo("/tmp/fifotest", 0660) < 0 && errno != EEXIST)
        printError("Nie mona utworzy FIFO");

    if ((fd = open("/tmp/fifotest", O_RDONLY)) < 0)
        printError("Nie mona otworzyc FIFO");

    sleep(15);
    // nie czytamy
    close(fd);
    unlink("/tmp/fifotest");
    
}