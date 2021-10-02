#include <sys/syscall.h>
#include <unistd.h>

void exit(int status)
{
    _syscall_1(SYS_EXIT, status);
}