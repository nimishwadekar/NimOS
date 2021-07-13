#include <stdarg.h>
#include "../String.hpp"
#include "Renderer.hpp"

Renderer::Renderer(Framebuffer framebuffer, PSF1 *font, uint32_t foregroundColour, uint32_t backgroundColour) :
    Buffer(framebuffer), Font(font), ForegroundColour(foregroundColour), BackGroundColour(backgroundColour)
{
    Cursor = {0, 0};
}

char FormattedStringBuffer[200];
void Renderer::Printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    FormatString(FormattedStringBuffer, format, args);
    va_end(args);

    for(unsigned int i = 0; FormattedStringBuffer[i] != 0; i++)
    {
        if(FormattedStringBuffer[i] == '\n')
        {
            Cursor.X = 0;
            Cursor.Y += 16;
            continue;
        }
        else if(FormattedStringBuffer[i] == '\t')
        {
            Cursor.X += 64;
            Cursor.X /= 64;
            Cursor.X *= 64;
            if(Cursor.X + 8 > Buffer.Width)
            {
                Cursor.X = 0;
                Cursor.Y += 16;
            }
            continue;
        }

        PutChar(FormattedStringBuffer[i]);
    }
}

void Renderer::PrintErrorf(const char *format, ...)
{
    uint32_t oldForegroundColour = ForegroundColour;
    SetForegroundColour(0xFF616D); // Shade of red

    va_list args;
    va_start(args, format);
    FormatString(FormattedStringBuffer, format, args);
    va_end(args);

    for(unsigned int i = 0; FormattedStringBuffer[i] != 0; i++)
    {
        if(FormattedStringBuffer[i] == '\n')
        {
            Cursor.X = 0;
            Cursor.Y += 16;
            continue;
        }
        else if(FormattedStringBuffer[i] == '\t')
        {
            Cursor.X += 64;
            Cursor.X /= 64;
            Cursor.X *= 64;
            if(Cursor.X + 8 > Buffer.Width)
            {
                Cursor.X = 0;
                Cursor.Y += 16;
            }
            continue;
        }

        PutChar(FormattedStringBuffer[i]);
    }
    MainRenderer.SetForegroundColour(oldForegroundColour);
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
    Cursor.X = xOffset + 8;
    Cursor.Y = yOffset;
    if(Cursor.X + 8 > Buffer.Width)
    {
        Cursor.X = 0;
        Cursor.Y += 16;
    }
}

void Renderer::PutChar(const char character)
{
    uint8_t *glyphPtr = &Font->Glyphs + character * Font->BytesPerGlyph;
    for(uint32_t y = Cursor.Y; y < Cursor.Y + 16; y++)
    {
        for(uint32_t x = Cursor.X; x < Cursor.X + 8; x++)
        {
            if((*glyphPtr & (0b10000000 >> (x - Cursor.X))) > 0)
            {
                PutPixel(x, y, ForegroundColour);
            }
        }
        glyphPtr += 1;
    }
    Cursor.X += 8;
    if(Cursor.X + 8 > Buffer.Width)
    {
        Cursor.X = 0;
        Cursor.Y += 16;
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

void Renderer::ClearScreen()
{
    for(uint32_t y = 0; y < Buffer.Height; y++)
    {
        for(uint32_t x = 0; x < Buffer.Width; x++)
        {
            PutPixel(x, y, BackGroundColour);
        }
    }
    Cursor.X = Cursor.Y = 0;
}

// The main renderer for the kernel.
Renderer MainRenderer(Framebuffer(0, (Framebuffer::FBType)0, 0, 0, 0, 0), 0, 0, 0);