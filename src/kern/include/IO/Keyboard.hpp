#pragma once

#include <stdint.h>
#include <IO/KeyboardScanCodes.hpp>

enum class SpecialKeys
{
    LCTRL = 1,
    LSHIFT,
    LALT,
    LGUI,
    RCTRL,
    RSHIFT,
    RALT,
    RGUI
};

#define KB_BUFFER_CAPACITY 16

class KeyboardBuffer
{
    public:
    uint8_t Modifier;
    uint8_t Left;
    uint8_t Right;
    char Buffer[KB_BUFFER_CAPACITY];

    KeyboardBuffer(void);
    void RegisterKeyPress(const uint8_t scanCode);
    bool IsModifierSet(const SpecialKeys key);
    void SetModifier(const SpecialKeys key);
    void ClearModifier(const SpecialKeys key);
    bool IsEmpty(void);
    char Dequeue(void);
};

// Lets kernel know about key-press.
void HandleKeyboard(const uint8_t scanCode);

extern KeyboardBuffer KBBuffer;