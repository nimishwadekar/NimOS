#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Program args:\n");
    for(int i = 0; i < argc; i++)
    {
        printf("%s, ", argv[i]);
    }
    printf("\n");

    return 1234;
}