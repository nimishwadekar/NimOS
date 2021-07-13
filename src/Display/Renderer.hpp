#pragma once

#include <stdint.h>
#include "Framebuffer.hpp"
#include "../Fonts/PSF.hpp"

struct Point
{
    uint32_t X;
    uint32_t Y;
};

#define COLOUR_WHITE    0xFFFFFFFF
#define COLOUR_BLACK    0xFF000000
#define COLOUR_RED      0xFFFF0000
#define COLOUR_GREEN    0xFF00FF00
#define COLOUR_BLUE     0xFF0000FF

// A wrapper class over the Framebuffer.
class Renderer
{
    public:
    Framebuffer Buffer;
    Point Cursor;
    PSF1 *Font; // The PSF1 font used to print strings.
    uint32_t ForegroundColour;
    uint32_t BackGroundColour;

    Renderer(Framebuffer framebuffer, PSF1 *font, uint32_t foregroundColour, uint32_t backgroundColour);
    void Printf(const char *format, ...);
    void PrintErrorf(const char *format, ...);
    void PutChar(const uint32_t xOffset, const uint32_t yOffset, const char character);
    void PutChar(const char character);
    void PutPixel(const uint32_t xOffset, const uint32_t yOffset, const uint32_t colour);
    void SetForegroundColour(const uint32_t colour);
    void SetBackgroundColour(const uint32_t colour);
    void SetCursor(const uint32_t xOffset, const uint32_t yOffset);
    void ClearScreen();
};

extern Renderer MainRenderer;