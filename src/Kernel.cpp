#include "Common.hpp"
#include "Display/Renderer.hpp"
#include "String.hpp"

void KernelStart()
{
    MainRenderer.ClearScreen();
    MainRenderer.Printf("Started Kernel.\n");

    unsigned a = 5;
    char c = 'A';
    MainRenderer.Printf("a = %u\t c = %u\thex = %x\n", a, c, 0xFF001C);
    MainRenderer.Printf("hex\tfull = %h\n", 0xFF001C);

    MainRenderer.Printf("\n\n\n");
    for(int i = 0; i < 50; i++) MainRenderer.Printf("Hellovo");

    MainRenderer.Printf("\n\n\n");

    int *addr = (int*) 0xF00000EE00000000;
    MainRenderer.PrintErrorf("WARNING: FATAL ERROR. FAULTY ADDRESS: 0x%h\n", addr);
    MainRenderer.Printf("Never mind, it's nothing.\n");

    while(true);
}