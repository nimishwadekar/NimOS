#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

extern void _heap_initialize(void *heapAddress, uint64_t pageCount);

int main()
{
    _heap_initialize((void*) 0x500000000, 1);


    FILE *f = fopen("anotherDirectory/src/hello.c", "r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = calloc(200, 1);
    printf("eof: %d\n", feof(f));
    printf("Read = %ld\n", fread(buf, 1, size, f));
    printf("eof: %d\n", feof(f));
    fclose(f);

    printf("\n\n-------------");
    return 0;
}