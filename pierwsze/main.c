
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]){

	if (argc != 3){

		printf("Wymagane sa 2 argumenty\n");
		return 2;
	}


	printf("%d\n", atoi(argv[1]) + atoi(argv[2]));

}


