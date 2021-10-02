#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("argc = %d\nargv = ", argc);
    for(int i = 0; i < argc; i++) printf("%s ", argv[i]);
    printf("%p\n\n", argv[argc]);

    printf("PID: %d\nPPID = %d\n", getpid(), getppid());
}