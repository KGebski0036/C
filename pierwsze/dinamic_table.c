#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("2 argument is necessary\n");
		return 2;
	}

	int number = atoi(argv[1]);

	if (number > 100 || number < 1)
	{
		printf("Wrong number of members\n");
		return 2;
	}

	int lenght_word = strlen(argv[2]);

	char **tab = (char **)malloc(sizeof(char *) * (number + 1));

	for (int i = 0; i < number; i++)
	{
		tab[i] = (char *)malloc(sizeof(char) * (lenght_word + 1 + 3 + 2));
		sprintf(tab[i], "%s %d\n", argv[2], i);
	}

	tab[number] = 0;

	for (int i = 0; i < number; i++)
	{
		printf("%s", tab[i]);
	}
}
