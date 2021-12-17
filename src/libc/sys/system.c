#include <sys/syscall.h>
#include <sys/system.h>

void shutdown(void)
{
    _syscall_0(SYS_SHUTDOWN);
}