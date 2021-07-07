#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"

// Kernel's main function.
void KernelStart(void)
{
    #ifdef LOGGING
    Logf("Kernel begin.\n");
    #endif

    MainRenderer.ClearScreen();
    MainRenderer.Printf("Kernel initialized.\n");

    while(true);
}