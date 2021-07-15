#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"
#include "Memory/Heap.hpp"
#include "IO/KeyboardScanCodes.hpp"
#include "IO/Keyboard.hpp"

// Kernel's main function.
void KernelStart(void)
{
    #ifdef LOGGING
    Logf("Kernel initialized.\n");
    #endif

    //MainRenderer.ClearScreen();
    MainRenderer.Printf("Kernel initialized.\n\n");

    while(true)
    {
        asm volatile("hlt");
        while(!KBBuffer.IsEmpty())
        {
            MainRenderer.PutChar(KBBuffer.Dequeue());
        }
    }
}