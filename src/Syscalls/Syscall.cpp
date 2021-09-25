#include <stdint.h>
#include <Display/Renderer.hpp>
#include <Syscalls/Stdio.hpp>
#include <Syscalls/Syscall.hpp>

// Add parameters and call syscall 1 from program.asm

extern "C" void SyscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9, uint64_t rax)
{
    Registers regs;
    regs.RDI = rdi;
    regs.RSI = rsi;
    regs.RDX = rdx;
    regs.R10 = r10;
    regs.R8 = r8;
    regs.R9 = r9;
    regs.RAX = rax;

    switch(rax)
    {
        case SYS_PRINTN: SysPrintN(&regs); break;
        case SYS_PRINT: SysPrint(&regs); break;
        case SYS_INPUTN: SysInputN(&regs); break;
        case SYS_INPUT: SysInput(&regs); break;
    }

    rax = regs.RAX; // Set the return value on the stack.
}