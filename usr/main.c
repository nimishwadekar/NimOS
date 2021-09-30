#include <stdio.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern void _heap_initialize(void *heapAddress, uint64_t pageCount);

int main()
{
    char buff[50] = {};
    _heap_initialize((void*) 0x400000000, 1);
    char *buf = calloc(100, sizeof(char));
    strcpy(buf, "     hello my       name is   nimish wadekar           ");
    //_syscall_1(SYS_PRINT, (int64_t) strstr("Hello World", "llo"));//lltoa((long long) (strdup(buf) == buf), buff + 10, 10));
    char *tok = strtok(buf, " ");
    while(tok != NULL)
    {
        _syscall_1(SYS_PRINT, (int64_t) tok);
        _syscall_1(SYS_PRINT, (int64_t) "->  0x");
        tok = strtok(NULL, " ");
        _syscall_1(SYS_PRINT, (int64_t) lltoa((long long) tok, buff + 10, 16));
        _syscall_1(SYS_PRINT, (int64_t) "\n");
    }
    _syscall_1(SYS_PRINT, (int64_t) "\nDone\n\n");

    return 0;
}