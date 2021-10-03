#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
    int c;
    char *argv[] = {"arg1", "arg2", "arg3", "arg4", "arg5", "arg6", "arg7", "arg8", NULL};
    spawnv("usr/spawn.elf", argv, &c);
    printf("Exited with code %d\n", c);

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