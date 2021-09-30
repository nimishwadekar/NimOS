#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

extern void _heap_initialize(void *heapAddress, uint64_t pageCount);

int main()
{
    int64_t pid = _syscall_0(SYS_PPID);
    printf("PPID = %d\n", pid);
    printf("\n\n-------------");

    return 0;
}