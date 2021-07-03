#include "Common.hpp"
#include "Display/Renderer.hpp"

void KernelStart()
{
    MainRenderer.Printf("Started Kernel.\n");

    while(true);
}