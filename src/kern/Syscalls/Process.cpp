#include <Syscalls/Process.hpp>
#include <Tasking/Process.hpp>
#include <Usermode/ELF.hpp>

#include <Display/Renderer.hpp>

/**
 *     DUPLICATE PAGE TABLES
 */ 

void SysExec(Registers *regs)
{
    ELF::LoadInfo info = ELF::LoadELF((void*) regs->RDI);
    Process *p = ProcessTop - 1;
    printf("Old process: 0x%x, %u pages\n", p->StartAddr, p->PageCount);
    printf("New process: 0x%x, %u pages\n", info.FirstAddress, info.PageCount);

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