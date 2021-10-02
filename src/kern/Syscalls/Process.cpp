#include <String.hpp>
#include <Syscalls/Process.hpp>
#include <Tasking/Process.hpp>
#include <Usermode/ArgV.hpp>
#include <Usermode/ELF.hpp>
#include <Usermode/Usermode.hpp>

#include <Display/Renderer.hpp>

/**
 *     DUPLICATE PAGE TABLES
 */ 

extern uint64_t STATE_RIP;
extern uint64_t STATE_RBX;
extern uint64_t STATE_RBP;
extern uint64_t STATE_RSP;
extern uint64_t STATE_R12;
extern uint64_t STATE_R13;
extern uint64_t STATE_R14;
extern uint64_t STATE_R15;
extern uint64_t STATE_RFLAGS;


void SysExec(Registers *regs)
{
    char **argv = (char**) regs->RSI;
    int argc = 0;
    char *argbuf = ArgvBuffer;
    while(*argv)
    {
        if(argbuf - ArgvBuffer >= ARGV_BUFFER_SIZE) break;
        strcpy(*argv, argbuf);
        argbuf += strlen(*argv) + 1;
        argv++;
        argc++;
    }

    ELF::LoadInfo info = ELF::LoadELF((void*) regs->RDI);
    PopProcess();
    if(PushProcess(info.Entry, info.FirstAddress, info.PageCount) == -1)
    {
        regs->RAX = (uint64_t) -1;
        return;
    }
    Process *newProcess = PeekProcess();

    uint64_t *stack = (uint64_t*) newProcess->StackTop - (argc + 2);
    *stack = argc;
    argbuf = ArgvBuffer;
    for(int i = 1; i <= argc; i++)
    {
        *(stack + i) = (uint64_t) argbuf;
        argbuf += strlen(argbuf) + 1;
    }
    *(stack + argc + 1) = 0;
    newProcess->StackTop = stack;

    JumpToUserAddress_Syscall(stack, newProcess->HeapBase, info.Entry);
}


void SysSpawn(Registers *regs)
{

}


void SysExit(Registers *regs)
{

}


void SysPID(Registers *regs)
{
    regs->RAX = ProcessCount;
}


void SysPPID(Registers *regs)
{
    regs->RAX = ProcessCount - 1;
}