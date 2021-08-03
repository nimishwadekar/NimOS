#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"
#include "Memory/Heap.hpp"
#include "IO/Keyboard.hpp"
#include "Scheduling/PIT/PIT.hpp"

#include "IO/Port.hpp"

// Kernel's main function.
void KernelStart(void)
{
    //MainRenderer.ClearScreen();
    printf("Kernel initialized.\n\n");

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