#include <stdio.h>
#include <sys/syscall.h>

int main()
{
    const char *str = "Hello C Land!";
    _syscall_1(SYS_SETBACK, 0);
    _syscall_3(SYS_DRAWP, 50, 50, 0xFFFF);
    _syscall_5(SYS_DRAWL, 100, 200, 300, 400, 0xFFFF);
    _syscall_2(SYS_PRINTN, (int64_t) str, 4);

    return 0;
}