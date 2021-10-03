#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

int main()
{
    int c;
    char *argv[] = {"arg1", "arg2", "arg3", "arg4", "arg5", "arg6", "arg7", "arg8", NULL};

    char *shm = shmat(171);
    printf("Shm at 0x%p\n", shm);
    for(int i = 0; i < 26; i++) shm[i] = 'A' + i;
    shm[26] = 0;

    spawnv("usr/spawn.elf", argv, &c);
    printf("\nExited with code %d\n", c);

    printf("Modified shm: %s\n", shm);

    shmdt();

    printf("\n\n-------------\n");
    while(1);
}
/**
 * 
 * REMOVE PRINTS FROM KERNEL.
 * 
 * REMOVE HANGS DUE TO ERRORS FROM KERNEL.
 * 
 * 
 */