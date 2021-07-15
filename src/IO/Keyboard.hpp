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

#define KB_BUFFER_CAPACITY 4

class KeyboardBuffer
{
    public:
    uint8_t Modifier;
    uint8_t Count;
    bool Error;
    uint8_t Buffer[KB_BUFFER_CAPACITY];

    KeyboardBuffer(void);
    void RegisterKeyPress(const uint8_t scanCode);
    void Remove(const uint8_t index);
    bool IsModifierSet(const SpecialKeys key);
    void SetModifier(const SpecialKeys key);
    void ClearModifier(const SpecialKeys key);
    bool IsEmpty(void);
    uint8_t Front(void);
    uint8_t Dequeue(void);
};

// Lets kernel know about key-press.
void HandleKeyboard(const uint8_t scanCode);

extern KeyboardBuffer KBBuffer;