#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    printf("2 arguments is necessary\n");
    return 2;
  }

  printf("%d\n", atoi(argv[1]) + atoi(argv[2]));
}
