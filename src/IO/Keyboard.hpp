#pragma once

#include <stdint.h>
#include "KeyboardScanCodes.hpp"

enum class SpecialKeys
{
    LCTRL = 0,
    LSHIFT = 1,
    LALT = 2,
    LGUI = 3,
    RCTRL = 4,
    RSHIFT = 5,
    RALT = 6,
    RGUI = 7
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