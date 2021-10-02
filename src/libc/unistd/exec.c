#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

int execv(const char *pathname, char *const argv[])
{
    FILE *file = fopen(pathname, "r");
    if(!file) return -1;
    void *buf = malloc(file->length);
    if(fread(buf, 1, file->length, file) != file->length)
    {
        free(buf);
        return -1;
    }

    _syscall_2(SYS_EXEC, (int64_t) buf, (int64_t) argv);
    return -1;
}


int spawnv(const char *pathname, char *const argv[])
{

}