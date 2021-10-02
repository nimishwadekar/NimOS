#include <Syscalls/Process.hpp>
#include <Tasking/Process.hpp>
#include <Usermode/ELF.hpp>
#include <Usermode/Usermode.hpp>

#include <Display/Renderer.hpp>

/**
 *     DUPLICATE PAGE TABLES
 */ 

void SysExec(Registers *regs)
{
    ELF::LoadInfo info = ELF::LoadELF((void*) regs->RDI);
    PopProcess();
    if(PushProcess(info.Entry, info.FirstAddress, info.PageCount) == -1)
    {
        regs->RAX = (uint64_t) -1;
        return;
    }
    Process *newProcess = PeekProcess();

    uint64_t *stack = (uint64_t*) newProcess->StackTop - 2;
    *stack = 0;
    *(stack + 1) = 0;

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