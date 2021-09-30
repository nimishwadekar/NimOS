#include <stdio.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>

extern void _heap_initialize(void *heapAddress, uint64_t pageCount);

int main()
{
    char buff[50] = {};
    /*char *s = (char*) _syscall_3(SYS_MMAP, (int64_t) 0x500000000, (int64_t) 1, 0);
    s[0] = 'H';
    s[1] = 0;
    //_syscall_1(SYS_PRINT, (int64_t) lltoa((long long)s, buff, 16));
    _syscall_1(SYS_PRINT, (int64_t) s);
    return 0; */

    _heap_initialize((void*) 0x400000000, 1);
    char *buf = malloc(10);
    buf[0] = 'H';
    buf[1] = 'e';
    buf[2] = 'a';
    buf[3] = 'p';
    buf[4] = 0;
    _syscall_1(SYS_PRINT, (int64_t) lltoa((long long) buf, buff + 10, 16));

    return 0;
}