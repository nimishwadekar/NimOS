#include <stdint.h>
#include <Display/Renderer.hpp>
#include <Syscalls/Display.hpp>
#include <Syscalls/FileIO.hpp>
#include <Syscalls/Memory.hpp>
#include <Syscalls/StdIO.hpp>
#include <Syscalls/Syscall.hpp>
#include <Syscalls/Timer.hpp>

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

        case SYS_FOPEN: SysFOpen(&regs); break;
        case SYS_FCLOSE: SysFClose(&regs); break;
        case SYS_FREAD: SysFRead(&regs); break;
        case SYS_FWRITE: SysFWrite(&regs); break;
        case SYS_FSEEK: SysFSeek(&regs); break;
        case SYS_FTELL: SysFTell(&regs); break;

        case SYS_MMAP: SysMMap(&regs); break;

        case SYS_SLEEP: SysSleep(&regs); break;

        case SYS_GETFORE: SysGetFore(&regs); break;
        case SYS_SETFORE: SysSetFore(&regs); break;
        case SYS_GETBACK: SysGetBack(&regs); break;
        case SYS_SETBACK: SysSetBack(&regs); break;
        case SYS_GETRES: SysGetRes(&regs); break;
        case SYS_GETCRSR: SysGetCrsr(&regs); break;
        case SYS_SETCRSR: SysSetCrsr(&regs); break;
        case SYS_DRAWP: SysDrawP(&regs); break;
        case SYS_DRAWL: SysDrawL(&regs); break;
        case SYS_DRAWR: SysDrawR(&regs); break;

        default: regs.RAX = -1; // Invalid system call.
    }

    rax = regs.RAX; // Set the return value on the stack.
}