#include "Renderer.hpp"

Renderer MainRenderer;

Renderer::Renderer() { }

Renderer::Renderer(Framebuffer framebuffer, PSF1 *font, uint32_t foregroundColour, uint32_t backgroundColour) :
    Buffer(framebuffer), Font(font), ForegroundColour(foregroundColour), BackGroundColour(backgroundColour)
{
    Cursor = {0, 0};
}

void Renderer::Printf(const char *format)
{
    for(unsigned int i = 0; format[i] != 0; i++)
    {
        if(format[i] == '\n')
        {
            Cursor.X = 0;
            Cursor.Y += 16;
            continue;
        }
        PutChar(Cursor.X, Cursor.Y, format[i]);
        Cursor.X += 8;
        if(Cursor.X + 8 > Buffer.Width)
        {
            Cursor.X = 0;
            Cursor.Y += 16;
        }
    }
}

void Renderer::PutChar(const uint32_t xOffset, const uint32_t yOffset, const char character)
{
    uint8_t *glyphPtr = &Font->Glyphs + character * Font->BytesPerGlyph;
    for(uint32_t y = yOffset; y < yOffset + 16; y++)
    {
        for(uint32_t x = xOffset; x < xOffset + 8; x++)
        {
            if((*glyphPtr & (0b10000000 >> (x - xOffset))) > 0)
            {
                PutPixel(x, y, ForegroundColour);
            }
        }
        glyphPtr += 1;
    }
}

void Renderer::PutPixel(const uint32_t xOffset, const uint32_t yOffset, const uint32_t colour)
{
    *(Buffer.BaseAddress + (yOffset * Buffer.PixelsPerScanLine) + xOffset) = colour;
}

void Renderer::SetForegroundColour(const uint32_t colour)
{
    ForegroundColour = colour;
}

void Renderer::SetBackgroundColour(const uint32_t colour)
{
    BackGroundColour = colour;
}

void Renderer::SetCursor(const uint32_t xOffset, const uint32_t yOffset)
{
    Cursor.X = xOffset;
    Cursor.Y = yOffset;
}