#include <stdarg.h>
#include <Display/Renderer.hpp>
#include <Logging.hpp>
#include <Memory/Memory.hpp>
#include <String.hpp>

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
        PutChar(FormattedStringBuffer[i]);
    }

    #ifdef LOGGING
    logf(FormattedStringBuffer);
    #endif
}

void Renderer::PrintErrorf(const char *format, ...)
{
    uint32_t oldForegroundColour = ForegroundColour;
    SetForegroundColour(COLOUR_RED);

    va_list args;
    va_start(args, format);
    FormatString(FormattedStringBuffer, format, args);
    va_end(args);

    for(unsigned int i = 0; FormattedStringBuffer[i] != 0; i++)
    {
        PutChar(FormattedStringBuffer[i]);
    }
    SetForegroundColour(oldForegroundColour);

    #ifdef LOGGING
    logf("ERROR: ");
    logf(FormattedStringBuffer);
    #endif
}

void Renderer::PutChar(const int32_t xOffset, const int32_t yOffset, const char character)
{
    bool specialChar = true;
    // Special cases.
    switch(character)
    {
        case '\r': // Carriage Return
            Cursor.X = 0;
            break;

        case '\n': // Line feed
            Cursor.X = 0;
            Cursor.Y += 16;
            break;

        case '\b': // Backspace
            if(Cursor.X == 0 && Cursor.Y == 0) return;
            Cursor.X -= 8;
            if(Cursor.X < 0)
            {
                Cursor.Y -= 16;
                Cursor.X = Buffer.Width - 8;
            }
            for(int32_t y = Cursor.Y; y < Cursor.Y + 16; y++) for(int32_t x = Cursor.X; x < Cursor.X + 8; x++) PutPixel(x, y, BackGroundColour);
            break;
        
        case ' ':
            Cursor.X += 8;
            if(Cursor.X + 8 > Buffer.Width)
            {
                Cursor.X = 0;
                Cursor.Y += 16;
            }
            break;

        default: specialChar = false; break;
    }

    if(!specialChar)
    {
        uint8_t *glyphPtr = &Font->Glyphs + character * Font->BytesPerGlyph;
        for(int32_t y = yOffset; y < yOffset + 16; y++)
        {
            for(int32_t x = xOffset; x < xOffset + 8; x++)
            {
                PutPixel(x, y, BackGroundColour); // To erase an earlier pixel.
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

    if(Cursor.Y >= Buffer.Height)
    {
        ScrollUp(16);
        Cursor.Y -= 16;
    }
}

void Renderer::PutChar(const char character)
{
    PutChar(Cursor.X, Cursor.Y, character);
}

void Renderer::PutPixel(const int32_t xOffset, const int32_t yOffset, const uint32_t colour)
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

void Renderer::SetCursor(const int32_t xOffset, const int32_t yOffset)
{
    Cursor.X = xOffset;
    Cursor.Y = yOffset;
}

void Renderer::ClearScreen()
{
    for(int32_t y = 0; y < Buffer.Height; y++)
    {
        memset32(Buffer.BaseAddress + Buffer.PixelsPerScanLine * y,
            BackGroundColour, Buffer.PixelsPerScanLine);
    }
    Cursor.X = Cursor.Y = 0;
}

void Renderer::ScrollUp(const int32_t pixels)
{
    for(int32_t y = 0; y < Buffer.Height - pixels; y++)
    {
         memcpy(Buffer.BaseAddress + Buffer.PixelsPerScanLine * (pixels + y), 
            Buffer.BaseAddress + Buffer.PixelsPerScanLine * y,
            Buffer.PixelsPerScanLine << 2);
    }
    memset32(Buffer.BaseAddress + Buffer.PixelsPerScanLine * (Buffer.Height - pixels),
        BackGroundColour, Buffer.PixelsPerScanLine * pixels);
}

// The main renderer for the kernel.
Renderer MainRenderer(Framebuffer(0, (Framebuffer::FBType)0, 0, 0, 0, 0), 0, 0, 0);

void printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    FormatString(FormattedStringBuffer, format, args);
    va_end(args);

    for(unsigned int i = 0; FormattedStringBuffer[i] != 0; i++)
    {
        MainRenderer.PutChar(FormattedStringBuffer[i]);
    }

    #ifdef LOGGING
    logf(FormattedStringBuffer);
    #endif
}

void errorf(const char *format, ...)
{
    uint32_t oldForegroundColour = MainRenderer.ForegroundColour;
    MainRenderer.SetForegroundColour(COLOUR_RED);

    va_list args;
    va_start(args, format);
    FormatString(FormattedStringBuffer, format, args);
    va_end(args);

    for(unsigned int i = 0; FormattedStringBuffer[i] != 0; i++)
    {
        MainRenderer.PutChar(FormattedStringBuffer[i]);
    }
    MainRenderer.SetForegroundColour(oldForegroundColour);

    #ifdef LOGGING
    logf("ERROR: ");
    logf(FormattedStringBuffer);
    #endif
}