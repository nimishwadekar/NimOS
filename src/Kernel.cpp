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
#include <Tasking/Process.hpp>
#include <Usermode/ELF.hpp>
#include <Usermode/Usermode.hpp>

// Kernel's main function.
void KernelStart(void)
{
    printf("Kernel initialized.\n\n");

    void *userStack = FrameAllocator.RequestPageFrame();
    PagingManager.MapPage((void*) USER_STACK_TOP, userStack);

    FILE *program = VFSOpenFile("usr/main.elf", "r");    
    uint8_t *programAddress = (uint8_t*) KernelHeap.Malloc(program->Length);
    if(VFSReadFile(program, programAddress, program->Length) != program->Length)
    {
        errorf("Program file read error.\n");
        while(true);
    }

    void *programEntry = ELF::LoadELF(programAddress);
    if(!programEntry)
    {
        errorf("Program file format incorrect.\n");
        while(true);
    }
    VFSCloseFile(program);

    /********* TEMPORARY *****************/
    VFSOpenFile("someOuterFile", "r");
    VFSOpenFile("anotherDirectory/file", "r");
    /********* TEMPORARY *****************/

    MainRenderer.SetBackgroundColour(USER_COLOUR_BACK);
    MainRenderer.SetForegroundColour(USER_COLOUR_FRONT);
    MainRenderer.ClearScreen();

    void *stackTop = (uint8_t*) USER_STACK_TOP + 0x1000;
    PushProcess(programEntry, stackTop);

    JumpToUserMode((void*) &SyscallEntry, stackTop, programEntry); // Does not return here.

    errorf("CAME BACK TO THE KERNEL!!!\n");
    while(true);
}