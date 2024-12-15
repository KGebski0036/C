// W części pierwszej (1a) powinien zostać napisany program, który będzie
// tworzyć w procesie głównym strumień danych, po czym podzieli się na dwa
// procesy, korzystając następnie z tego strumienia komunikacyjnego do
// przesłania danych – np. tekstu wczytanego z klawiatury lub dowolnych innych
// informacji, wyświetlonych następnie w drugim z procesów.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG "Hello World!!"

void child(int fd[2]){
    close(fd[0]);
    write(fd[1], MSG, strlen(MSG) + 1);
    close(fd[1]);
}

void parent(int fd[2]){

    char buffor[strlen(MSG) + 1];
    close(fd[1]);
    read(fd[0], buffor, strlen(MSG) + 1);
	printf("%s\n", buffor);
    close(fd[0]);
}

int main(void) {
    int fd[2];

    if (pipe(fd) < 0)
    {
        printf("Nie udao sie utworzyc strumienia");
        exit(1);
    }

    pid_t pid = fork();

    switch(pid){
        case -1:
            printf("Error");
            break;
        case 0:
            child(fd);
            break;
        default:
            parent(fd);
            break;
    }
    return 0;
}
