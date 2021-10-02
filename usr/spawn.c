#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("argc = %d\nargv = ", argc);
    for(int i = 0; i < argc; i++) printf("%s ", argv[i]);
    printf("%p\n", argv[argc]);

    while(1);
}