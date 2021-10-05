#include <stdarg.h>
#include <Display/Renderer.hpp>
#include <Logging.hpp>
#include <Memory/Memory.hpp>
#include <String.hpp>

Renderer::Renderer(Framebuffer framebuffer, PSF2 *font, uint32_t foregroundColour, uint32_t backgroundColour) :
    Buffer(framebuffer), Font(font), ForegroundColour(foregroundColour), BackGroundColour(backgroundColour)
{
    Cursor = {0, 0};
    FontWidth = font->Width;
    FontHeight = font->Height;
}

static bool IsLocked = false;
static Point LockedTopLeft, LockedBottomRight;

static inline bool InLock(Point p)
{
    return p.X >= LockedTopLeft.X && p.X < LockedBottomRight.X && p.Y >= LockedTopLeft.Y && p.Y < LockedBottomRight.Y;
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
            Cursor.Y += FontHeight;
            break;

        case '\b': // Backspace
            if(Cursor.X == 0 && Cursor.Y == 0) return;
            Cursor.X -= FontWidth;
            if(Cursor.X < 0)
            {
                Cursor.Y -= FontHeight;
                Cursor.X = Buffer.Width - FontWidth;
            }
            for(int32_t y = Cursor.Y; y < Cursor.Y + FontHeight; y++) 
                for(int32_t x = Cursor.X; x < Cursor.X + FontWidth; x++) 
                    *(Buffer.BaseAddress + (y * Buffer.PixelsPerScanLine) + x) = BackGroundColour;
            break;

        default: specialChar = false; break;
    }

    if(!specialChar)
    {
        uint8_t *glyphPtr = &Font->Glyphs + character * Font->BytesPerGlyph;
        uint8_t bytesPerLine = (FontWidth + 7) >> 3;
        uint32_t shift;
        for(int32_t y = yOffset; y < yOffset + FontHeight; y++)
        {
            for(int32_t x = xOffset; x < xOffset + FontWidth; x++)
            {
                *(Buffer.BaseAddress + (y * Buffer.PixelsPerScanLine) + x) = BackGroundColour;
                shift = x - xOffset;
                if((glyphPtr[shift >> 3] & (0b10000000 >> (shift & 0b111))))
                {
                    *(Buffer.BaseAddress + (y * Buffer.PixelsPerScanLine) + x) = ForegroundColour;
                }
            }
            glyphPtr += bytesPerLine;
        }

        Cursor.X = xOffset + FontWidth;
        Cursor.Y = yOffset;
        if(Cursor.X + FontWidth > Buffer.Width)
        {
            Cursor.X = 0;
            Cursor.Y += FontHeight;
        }
    }

    if(Cursor.Y + 2 * FontHeight >= Buffer.Height)
    {
        ScrollUp(FontHeight);
        Cursor.Y -= FontHeight;
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
    Cursor.X = xOffset - (xOffset % FontWidth);
    Cursor.Y = yOffset - (yOffset % FontHeight);
}

void Renderer::ClearScreen()
{
    for(int32_t y = 0; y < Buffer.Height; y++)
    {
        memset32(Buffer.BaseAddress + Buffer.PixelsPerScanLine * y,
            BackGroundColour, Buffer.Width);
    }
    Cursor.X = Cursor.Y = 0;
    IsLocked = false;
}

void Renderer::ScrollUp(const int32_t pixels)
{
    int32_t x, y;
    uint32_t *to, *from;
    for(y = 0; y < Buffer.Height - pixels; y++)
    {
        from = Buffer.BaseAddress + Buffer.PixelsPerScanLine * (pixels + y);
        to = Buffer.BaseAddress + Buffer.PixelsPerScanLine * y;
        for(x = 0; x < Buffer.Width; x++)
        {
            if(IsLocked && InLock({x, y})) continue;
            *(to + x) = *(from + x);
        }
    }

    for( ; y < Buffer.Height; y++)
    {
        memset32(Buffer.BaseAddress + Buffer.PixelsPerScanLine * y, BackGroundColour, Buffer.Width >> 2);
        to = Buffer.BaseAddress + Buffer.PixelsPerScanLine * y;
        for(x = 0; x < Buffer.Width; x++)
        {
            if(IsLocked && InLock({x, y})) continue;
            *(to + x) = BackGroundColour;
        }
    }
}

void Renderer::LockArea(Point topLeft, Point bottomRight)
{
    IsLocked = true;
    LockedTopLeft = topLeft;
    LockedBottomRight = bottomRight;
}

void Renderer::UnlockArea()
{
    IsLocked = false;
}

void Renderer::DrawCursor()
{
    for(int32_t y = Cursor.Y + FontHeight - CURSOR_THICKNESS; y < Cursor.Y + FontHeight; y++)
    {
        for(int32_t x = Cursor.X; x < Cursor.X + FontWidth; x++)
        {
            *(Buffer.BaseAddress + (y * Buffer.PixelsPerScanLine) + x) = ForegroundColour;
        }
    }
}

void Renderer::EraseCursor()
{
    for(int32_t y = Cursor.Y + FontHeight - CURSOR_THICKNESS; y < Cursor.Y + FontHeight; y++)
    {
        for(int32_t x = Cursor.X; x < Cursor.X + FontWidth; x++)
        {
            *(Buffer.BaseAddress + (y * Buffer.PixelsPerScanLine) + x) = BackGroundColour;
        }
    }
}

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


// The main renderer for the kernel.
Renderer MainRenderer(Framebuffer(0, (Framebuffer::FBType)0, 0, 0, 0, 0), 0, 0, 0);