#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <elf.h>

extern void _heap_initialize(void *heapAddress, uint64_t pageCount);

int main()
{
    _heap_initialize((void*) 0x500000000, 1);

    execv("usr/spawn.elf", NULL);

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