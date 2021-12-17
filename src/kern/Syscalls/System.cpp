#include <IO/Port.hpp>
#include <Syscalls/System.hpp>

void SysShutdown(Registers *regs)
{
    outw(0x604, 0x2000);
}