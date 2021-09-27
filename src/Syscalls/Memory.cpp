#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Syscalls/Memory.hpp>

void SysMMap(Registers *regs)
{
    if(regs->RDX == 1)
    {
        uint8_t *mem = (uint8_t*) FrameAllocator.RequestPageFrames(regs->RSI);
        for(uint64_t i = 0; i < regs->RSI; i++) 
            PagingManager.MapPage((uint8_t*) regs->RDI + i * 0x1000, mem + i * 0x1000);
    }
    else if(regs->RDX == 0)
        for(uint64_t i = 0; i < regs->RSI; i++) 
            PagingManager.MapPage((uint8_t*) regs->RDI + i * 0x1000, FrameAllocator.RequestPageFrame());
    regs->RAX = regs->RDI;
}