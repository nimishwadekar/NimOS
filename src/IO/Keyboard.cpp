#include "Keyboard.hpp"
#include "../Display/Renderer.hpp"

KeyboardBuffer KBBuffer;

void HandleKeyboard(const uint8_t scanCode)
{
    KBBuffer.RegisterKeyPress(scanCode);

    /* MainRenderer.Printf("%y  %u  %s  :  ", KBBuffer.Modifier, KBBuffer.Count, KBBuffer.Error ? "ERROR" : "NO-ERROR");
    for(int i = 0; i < KB_BUFFER_CAPACITY; i++) MainRenderer.Printf("%x  ", KBBuffer.Buffer[i]);
    MainRenderer.Printf("\n"); */

    if(KBBuffer.Error) // Hang system if error
    {
        asm volatile("cli");
        MainRenderer.PrintErrorf("KEYBOARD BUFFER OVERFLOW\n");
        while(true) asm volatile("hlt");
    }
}

KeyboardBuffer::KeyboardBuffer() : Modifier(0), Count(0)
{
    for(int i = 0; i < KB_BUFFER_CAPACITY; i++) Buffer[i] = 0;
}

static bool AltEngaged = false;

#define LCTRL_CODE_PRESS 0x1D
#define LSHIFT_CODE_PRESS 0x2A
#define LALT_CODE_PRESS 0x38
#define LGUI_CODE_PRESS_ALT 0x5B
#define RCTRL_CODE_PRESS_ALT 0x1D
#define RSHIFT_CODE_PRESS 0x36
#define RALT_CODE_PRESS_ALT 0x38
#define RGUI_CODE_PRESS_ALT 0x5C

#define LCTRL_CODE_RELEASE 0x9D
#define LSHIFT_CODE_RELEASE 0xAA
#define LALT_CODE_RELEASE 0xB8
#define LGUI_CODE_RELEASE_ALT 0xDB
#define RCTRL_CODE_RELEASE_ALT 0x9D
#define RSHIFT_CODE_RELEASE 0xB6
#define RALT_CODE_RELEASE_ALT 0xB8
#define RGUI_CODE_RELEASE_ALT 0xDC

void KeyboardBuffer::RegisterKeyPress(const uint8_t scanCode)
{
    if(scanCode == QWERTYKeyboard::ALTERNATE_PREFIX)
    {
        AltEngaged = true;
        return;
    }

    // Check for modifier keys.

    if(!AltEngaged)
    {
        switch(scanCode)
        {
            case LCTRL_CODE_PRESS:
                SetModifier(SpecialKeys::LCTRL);
                return;
            case LSHIFT_CODE_PRESS:
                SetModifier(SpecialKeys::LSHIFT);
                return;
            case LALT_CODE_PRESS:
                SetModifier(SpecialKeys::LALT);
                return;
            case RSHIFT_CODE_PRESS:
                SetModifier(SpecialKeys::RSHIFT);
                return;

            case LCTRL_CODE_RELEASE:
                ClearModifier(SpecialKeys::LCTRL);
                return;
            case LSHIFT_CODE_RELEASE:
                ClearModifier(SpecialKeys::LSHIFT);
                return;
            case LALT_CODE_RELEASE:
                ClearModifier(SpecialKeys::LALT);
                return;
            case RSHIFT_CODE_RELEASE:
                ClearModifier(SpecialKeys::RSHIFT);
                return;
        }
    }
    else
    {
        switch(scanCode)
        {
            case LGUI_CODE_PRESS_ALT:
                SetModifier(SpecialKeys::LGUI);
                AltEngaged = false;
                return;
            case RCTRL_CODE_PRESS_ALT:
                SetModifier(SpecialKeys::RCTRL);
                AltEngaged = false;
                return;
            case RALT_CODE_PRESS_ALT:
                SetModifier(SpecialKeys::RALT);
                AltEngaged = false;
                return;
            case RGUI_CODE_PRESS_ALT:
                SetModifier(SpecialKeys::RGUI);
                AltEngaged = false;
                return;

            case LGUI_CODE_RELEASE_ALT:
                ClearModifier(SpecialKeys::LGUI);
                AltEngaged = false;
                return;
            case RCTRL_CODE_RELEASE_ALT:
                ClearModifier(SpecialKeys::RCTRL);
                AltEngaged = false;
                return;
            case RALT_CODE_RELEASE_ALT:
                ClearModifier(SpecialKeys::RALT);
                AltEngaged = false;
                return;
            case RGUI_CODE_RELEASE_ALT:
                ClearModifier(SpecialKeys::RGUI);
                AltEngaged = false;
                return;
        }
    }

    for(int i = 0; i < Count; i++)
    {
        if(Buffer[i] + 0x80 == scanCode) // Key release code
        {
            Remove(i);
            return;
        }

        if(Buffer[i] == scanCode) return; // Already pressed
    }

    if(Count >= KB_BUFFER_CAPACITY) // overflow
    {
        if(!Error) Error = true;
        Count += 1;
        return;
    }

    Buffer[Count] = scanCode;
    Count += 1;
}

void KeyboardBuffer::Remove(const uint8_t index)
{
    for(int i = index; i < Count - 1; i++) Buffer[i] = Buffer[i + 1];
    for(int i = Count - 1; i < KB_BUFFER_CAPACITY; i++) Buffer[i] = 0;
    Count -= 1;
}

bool KeyboardBuffer::IsModifierSet(const SpecialKeys key)
{
    return (bool) (Modifier & (1 << (uint8_t) key));
}

void KeyboardBuffer::SetModifier(const SpecialKeys key)
{
    Modifier |= (1 << (uint8_t) key);
}

void KeyboardBuffer::ClearModifier(const SpecialKeys key)
{
    Modifier &= ~(1 << (uint8_t) key);
}

bool KeyboardBuffer::IsEmpty(void)
{
    return Count == 0;
}

uint8_t KeyboardBuffer::Front(void)
{
    return Buffer[0];
}

uint8_t KeyboardBuffer::Dequeue(void)
{
    uint8_t front = Buffer[0];
    Remove(0);
    return front;
}