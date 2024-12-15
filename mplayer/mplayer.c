#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void printError(const char *err);
void player(char *file);
void frontend(void);
void setupFIFO(int *fd);
int  getOptionFromUser(void);

int main(int argc, char **argv) {

    if (argc != 2)
        printError("Użycie programu: ./mplayer_frontend [plik.mp3]");

    if (mkfifo("/tmp/fifo_file", 0660) < 0 && errno != EEXIST)
        printError("Nie mona utworzy FIFO");

    pid_t pid = fork();

    switch(pid){
        case -1:
            printf("Error");
            break;
        case 0:
            player(argv[1]);
            break;
        default:
            frontend();
            break;
    }
}

void printError(const char *err) {
    fprintf(stderr, "%s\n", err);
    exit(1);
}

void player(char *file) {
    close(0);
    execlp("mplayer", "-msglevel=-1", "-msgcolor", "-really-quiet", "-input", "file=/tmp/fifo_file", file, NULL);
    printError("Nie udalo sie uruchomic playera");
}

void frontend(void) {

    int fd;
    setupFIFO(&fd);
    char *commands[] = {"quit\n", "MUTE\n", "PAUSE\n", "seek 5\n", "seek -5\n"};

    printf("\e[1;1H\e[2J");

    for(;;){

        int index = getOptionFromUser();

        if (index > -1 && index < 5)
        {
            if(!write(fd, commands[index], strlen(commands[index])))
                printError("Nieudany write");
        }
        else
            printf("Nieznana opcja\n");

        if (index == 0)
            break;
    }
    close(fd);
    unlink("/tmp/fifo_file");
}

void setupFIFO(int *fd) {
    if (((*fd) = open("/tmp/fifo_file", O_WRONLY)) < 0)
        printError("Nie mona otworzyc FIFO");
}

int  getOptionFromUser(void) {
    printf("1. mute\n2. pause\n3. seek 5s forward\n4. seek 5s backward\n0. quit\nType a commend:");
    char option;

    if(!scanf("%c", &option))
        printf("Blad wczytywania\n");
    printf("\e[1;1H\e[2J");
    while(option != '\n' && getchar() != '\n' );

    return (int)(option - '0');
}
