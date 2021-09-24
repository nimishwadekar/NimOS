#include <IO/Keyboard.hpp>
#include <Utility.hpp>

KeyboardBuffer KBBuffer;

void HandleKeyboard(const uint8_t scanCode)
{
    KBBuffer.RegisterKeyPress(scanCode);
}

KeyboardBuffer::KeyboardBuffer() : Modifier(0), Left(0), Right(0)
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

    char input = QWERTYKeyboard::TranslateScanCode(scanCode);
    if(input == 0) return;

    if(IsModifierSet(SpecialKeys::LSHIFT) || IsModifierSet(SpecialKeys::RSHIFT)) // Add all other shift-keys
    {
        input = toUpper(input);
    }

    Buffer[Right] = input;
    Right += 1;
    if(Right == KB_BUFFER_CAPACITY) Right = 0;
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
    return Left == Right;
}

char KeyboardBuffer::Dequeue(void)
{
    char front = Buffer[Left];
    Left += 1;
    if(Left == KB_BUFFER_CAPACITY) Left = 0;
    return front;
}