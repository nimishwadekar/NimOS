#include <stdint.h>
#include <Display/Renderer.hpp>
#include <Syscalls/Syscall.hpp>

// Add parameters and call syscall 1 from program.asm

extern "C" void SyscallHandler()
{
    printf("Here syscall %u\n", SYS_PRINT);
}