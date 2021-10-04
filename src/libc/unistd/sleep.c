#include <sys/syscall.h>
#include <unistd.h>

unsigned int sleep(unsigned int seconds)
{
	_syscall_2(SYS_SLEEP, seconds, 0);
    return 0;
}

double sleepms(double milliseconds)
{
    _syscall_2(SYS_SLEEP, milliseconds, 1);
    return 0;
}