#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
    char *argv[] = {"arg1", "arg2", "arg3", "arg4", "arg5", "arg6", "arg7", "arg8", NULL};
    execv("usr/spawn.elf", argv);

    printf("\n\n-------------");
    return 0;
}

/**
 * 
 * REMOVE PRINTS FROM KERNEL.
 * 
 * REMOVE HANGS DUE TO ERRORS FROM KERNEL.
 * 
 * 
 */