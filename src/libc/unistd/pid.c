#include <sys/syscall.h>
#include <unistd.h>

pid_t getpid(void)
{
    return (pid_t) _syscall_0(SYS_PID);
}


pid_t getppid(void)
{
    return (pid_t) _syscall_0(SYS_PPID);
}