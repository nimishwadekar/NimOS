#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("argc = %d\nargv = ", argc);
    for(int i = 0; i < argc; i++) printf("%s ", argv[i]);
    printf("%p\n\n", argv[argc]);

    printf("PID: %d\nPPID = %d\n\n", getpid(), getppid());

    char *shm = shmat(171);
    printf("Spawn shm: %s\n", shm);
    for(int i = 0; i < 26; i++) shm[i] += 32;

    shmdt();

    return 0;
}