#include "Keyboard.hpp"
#include "../Display/Renderer.hpp"

void KeyboardInterruptHandler(const uint8_t scanCode)
{
    MainRenderer.Printf("Scancode 0x%x\n", scanCode);
}