#include <IO/KeyboardScanCodes.hpp>

namespace QWERTYKeyboard
{
    const uint8_t ALTERNATE_PREFIX = 0xE0;
    const uint8_t RELEASE_OFFSET = 0x80;
    const char ScanCodeTable[]
    {
         0 ,  0 , '1', '2', // Null, Esc
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=','\b',  0 , // Backspace, Tab

        'q', 'w', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
        '\n', 0 , 'a', 's', // Enter, Left Ctrl

        'd', 'f', 'g', 'h',
        'j', 'k', 'l', ';',
        '\'', '`', 0 , '\\', // Left Shift

        'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',',
        '.', '/',  0 , '*', // Right Shift, Keypad *
         0 , ' ',  0 ,  0 , // Left Alt, Caps Lock, F1

         0 ,  0 ,  0 ,  0 , // F2, F3, F4, F5
         0 ,  0 ,  0 ,  0 , // F6, F7, F8, F9
         0 ,  0 ,  0 , '7', // F10, Num Lock, Scroll Lock, Keypad 7
        '8', '9', '-', '4', // All keypad

        '5', '6', '+', '1', // All keypad
        '2', '3', '0', '.', // All keypad
         0 ,  0 ,  0 ,  0 , // Null, Null, Null, F11
         0                  // F12
    };

    char TranslateScanCode(const uint8_t scanCode)
    {
        if(scanCode > 0x58) return 0;
        return ScanCodeTable[scanCode];
    }
}