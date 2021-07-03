#pragma once

#include "../Common.hpp"
#include "Framebuffer.hpp"
#include "../Fonts/PSF.hpp"

struct Point
{
    uint32_t X;
    uint32_t Y;
};

class Renderer
{
    public:
    Framebuffer Buffer;
    Point Cursor;
    PSF1 *Font;
    uint32_t ForegroundColour;
    uint32_t BackGroundColour;

    Renderer();
    Renderer(Framebuffer framebuffer, PSF1 *font, uint32_t foregroundColour, uint32_t backgroundColour);
    void Printf(const char *format);
    void PutChar(const uint32_t xOffset, const uint32_t yOffset, const char character);
    void PutPixel(const uint32_t xOffset, const uint32_t yOffset, const uint32_t colour);
    void SetForegroundColour(const uint32_t colour);
    void SetBackgroundColour(const uint32_t colour);
};

extern Renderer MainRenderer;