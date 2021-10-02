#include <syscall.h>

extern int64_t _Syscall();

int64_t _syscall_0(int64_t syscall)
{
    return _Syscall(syscall);
}


int64_t _syscall_1(int64_t syscall, int64_t arg0)
{
    return _Syscall(syscall, arg0);
}


int64_t _syscall_2(int64_t syscall, int64_t arg0, int64_t arg1)
{
    return _Syscall(syscall, arg0, arg1);
}


int64_t _syscall_3(int64_t syscall, int64_t arg0, int64_t arg1, int64_t arg2)
{
    return _Syscall(syscall, arg0, arg1, arg2);
}


int64_t _syscall_4(int64_t syscall, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3)
{
    return _Syscall(syscall, arg0, arg1, arg2, arg3);
}


int64_t _syscall_5(int64_t syscall, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4)
{
    return _Syscall(syscall, arg0, arg1, arg2, arg3, arg4);
}