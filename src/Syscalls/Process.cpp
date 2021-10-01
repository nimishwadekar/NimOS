#include <Syscalls/Process.hpp>
#include <Tasking/Process.hpp>

void SysExec(Registers *regs)
{

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