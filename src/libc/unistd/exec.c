#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

int execv(const char *pathname, char *const argv[])
{
    FILE *file = fopen(pathname, "r");
    if(!file) return -1; // File not found
    void *buf = malloc(file->length);
    if(fread(buf, 1, file->length, file) != file->length)
    {
        free(buf);
        return -2; // Read failed.
    }

    _syscall_2(SYS_EXEC, (int64_t) buf, (int64_t) argv);
    return -1;
}

int spawnv(const char *pathname, char *const argv[], int *exitcode)
{
    FILE *file = fopen(pathname, "r");
    if(!file) return -1; // File not found
    void *buf = malloc(file->length);
    if(fread(buf, 1, file->length, file) != file->length)
    {
        free(buf);
        return -2; // Read failed.
    }

    _syscall_3(SYS_SPAWN, (int64_t) buf, (int64_t) argv, (int64_t) exitcode);
    return 0;
}