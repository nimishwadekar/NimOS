#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"
#include "Memory/Heap.hpp"
#include "IO/Keyboard.hpp"
#include "Scheduling/PIT/PIT.hpp"
#include "IO/Port.hpp"
#include "FS/VFS.hpp"

char buf[1000] = {};
// Kernel's main function.
void KernelStart(void)
{
    //MainRenderer.ClearScreen();
    printf("Kernel initialized.\n\n");

    FILE file = VFSOpenFile("anotherDirectory/file");
    printf("File %s [%u] opened.\n", file.Name, file.Length);
    VFSReadFile(&file, buf, 10);
    printf("Read = %s\n", buf);
    VFSReadFile(&file, buf + 15, 10);
    printf("Read = %s\n", buf + 15);
    VFSReadFile(&file, buf + 15, 10);
    printf("Read = %s\n", buf + 15);
    VFSCloseFile(&file);

    /**********
     *  
     *  IMPLEMENT POINTER COUNTER.
     *  IMPLEMENT FSEEK(), FGETC().
     * 
     * *******/

    while(true)
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
    }
}