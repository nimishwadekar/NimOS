#pragma once

#include "../Common.hpp"

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
    uint32_t Width;
    uint32_t Height;
    uint32_t PixelsPerScanLine;

    Framebuffer();
    Framebuffer(uint32_t *baseAddress, FBType type, uint32_t size, uint32_t width, uint32_t height, uint32_t pixelsPerScanLine);
};