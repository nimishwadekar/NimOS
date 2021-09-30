#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern void _heap_initialize(void *heapAddress, uint64_t pageCount);

int main()
{
    int n;
    printf("P:%p\n%nend\n", &main, &n);
    printf("N = %d\n", n);
    printf("\n-------------");

    return 0;
}