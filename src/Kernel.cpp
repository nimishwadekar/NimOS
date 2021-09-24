#include <stdint.h>
#include <Display/Renderer.hpp>
#include <FS/VFS.hpp>
#include <IO/Keyboard.hpp>
#include <IO/PIT.hpp>
#include <IO/Port.hpp>
#include <Logging.hpp>
#include <Memory/Heap.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <String.hpp>
#include <Syscalls/Syscall.hpp>
#include <Usermode/Usermode.hpp>

extern "C" void BeginUserMode(uint32_t *fb);

// Kernel's main function.
void KernelStart(void)
{
    //MainRenderer.ClearScreen();
    printf("Kernel initialized.\n\n");

    void *userStack = FrameAllocator.RequestPageFrame();
    PagingManager.MapPage(userStack, userStack);

    uint8_t *programAddress = (uint8_t*) FrameAllocator.RequestPageFrame();
    PagingManager.MapPage(programAddress, programAddress);
    FILE program = VFSOpenFile("programs/program.bin");
    VFSReadFile(&program, programAddress, program.Length);

    JumpToUserMode((void*) &SyscallEntry, (uint8_t*) userStack + 0x1000, programAddress); // Does not return here.

    /* while(true)
    {
        asm volatile("hlt");
        while(!KBBuffer.IsEmpty())
        {
            char c = KBBuffer.Dequeue();
            if(c == 'q')
            {
                outw(0x604, 0x2000);
            }
        }
    } */
}