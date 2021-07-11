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

    void *malloced = Malloc(0x8000);
    MainRenderer.Printf("Malloced 0x%x\n", malloced);
    void *alloc = Malloc(0x8000);
    MainRenderer.Printf("Malloced 0x%x\n", alloc);
    malloced = Malloc(0x200);
    MainRenderer.Printf("Malloced 0x%x\n", malloced);
    malloced = Malloc(0x200);
    MainRenderer.Printf("Malloced 0x%x\n", malloced);
    Free(alloc);
    MainRenderer.Printf("Malloced 0x%x\n", Malloc(0x8001));
    MainRenderer.Printf("Malloced 0x%x\n", Malloc(0x6000));
    MainRenderer.Printf("Malloced 0x%x\n", Malloc(0x400));

    MainRenderer.Printf("\nKernel ended.\n");

    while(true);
}