#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"
#include "Memory/Heap.hpp"
#include "IO/Keyboard.hpp"
#include "Scheduling/PIT/PIT.hpp"
#include "IO/Port.hpp"
#include "FS/VFS.hpp"

char buf[10000] = {};
// Kernel's main function.
void KernelStart(void)
{
    //MainRenderer.ClearScreen();
    printf("Kernel initialized.\n\n");

    FILE file = VFSOpenFile("anotherDirectory/file");
    printf("File %s [%u] opened.\n", file.Name, file.Length);
    char c;
    int i = 200;
    printf("char = %u\n", VFSGetChar(&file));
    /* while((c = VFSGetChar(&file)) != FILE_EOF)
    {
        logf("%c", c);
        if(i-- == 0) break;
    } */
    VFSCloseFile(&file);
    printf("done");

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