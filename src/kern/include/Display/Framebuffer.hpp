#pragma once

#include <stdint.h>

// The Framebuffer.
class Framebuffer
{
    public:
    uint32_t *BaseAddress;
    enum FBType
    {
        ARGB = 0,
        RGBA = 1,
        ABGR = 2,
        BGRA = 3
    } Type;
    uint32_t Size;
    int32_t Width;
    int32_t Height;
    int32_t PixelsPerScanLine;

    Framebuffer(uint32_t *baseAddress, FBType type, uint32_t size, int32_t width, int32_t height, int32_t pixelsPerScanLine);
};