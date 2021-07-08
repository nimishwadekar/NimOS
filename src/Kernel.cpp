#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"

// Kernel's main function.
void KernelStart(void)
{
    #ifdef LOGGING
    Logf("Kernel initialized.\n");
    #endif

    MainRenderer.ClearScreen();
    MainRenderer.Printf("Kernel initialized.\n");

    int *a = (int*) 0xFFFFFF0000000000;
    *a = 5;

    MainRenderer.Printf("Kernel ended.\n");

    while(true);
}