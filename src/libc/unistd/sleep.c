#include <sys/syscall.h>
#include <unistd.h>

unsigned int sleep(unsigned int seconds)
{
	_syscall_2(SYS_SLEEP, seconds, 0);
    return 0;
}