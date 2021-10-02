#include <Syscalls/Process.hpp>
#include <Tasking/Process.hpp>
#include <Usermode/ELF.hpp>

#include <Display/Renderer.hpp>

/**
 *     DUPLICATE PAGE TABLES
 */ 

void SysExec(Registers *regs)
{
    //void *program = ELF::LoadELF((void*) regs->RDI);
    Process *p = ProcessTop - 1;
    printf("Process: 0x%x, %u pages\n", p->StartAddr, p->PageCount);

    while(1);
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