#include <stdint.h>
#include <Display/Renderer.hpp>
#include <IO/Keyboard.hpp>
#include <Syscalls/StdIO.hpp>

void SysPrintN(Registers *regs)
{
    char *str = (char*) regs->RSI;
    for(uint64_t n = regs->R10; n > 0; str++, n--)
    {
        MainRenderer.PutChar(*str);
    }
    regs->RAX = 0;
}


void SysPrint(Registers *regs)
{
    char *str = (char*) regs->RSI;
    for( ; *str != 0; str++)
    {
        MainRenderer.PutChar(*str);
    }
    regs->RAX = 0;
}


void SysInputN(Registers *regs)
{
    uint64_t n = regs->R10;
    char *dst = (char*) regs->RDI;
    for( ; n > 0; n--, dst++)
    {
        while(KBBuffer.IsEmpty());

        *dst = KBBuffer.Dequeue();
        MainRenderer.PutChar(*dst);
    }
}


void SysInput(Registers *regs)
{
    char *dst = (char*) regs->RDI;
    do
    {
        while(KBBuffer.IsEmpty());

        *dst = KBBuffer.Dequeue();
        MainRenderer.PutChar(*dst);
    } while(*(dst++) != '\n');
}