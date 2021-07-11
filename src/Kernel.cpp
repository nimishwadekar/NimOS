#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"
#include "Memory/Heap.hpp"

// Kernel's main function.
void KernelStart(void)
{
    #ifdef LOGGING
    Logf("Kernel initialized.\n");
    #endif

    MainRenderer.ClearScreen();
    MainRenderer.Printf("Kernel initialized.\n");

    void *malloced = KernelHeap.Malloc(0x200);
    MainRenderer.Printf("Malloced 0x%x\n", malloced);

    MainRenderer.Printf("\nKernel ended.\n");

    while(true);
}