#include <stdint.h>
#include <Display/Renderer.hpp>
#include <Syscalls/Syscall.hpp>

extern "C" void SyscallHandler()
{
    printf("Here syscall %u\n", SYS_PRINT);
}