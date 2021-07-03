#include "Framebuffer.hpp"

Framebuffer::Framebuffer() { }

Framebuffer::Framebuffer(uint32_t *baseAddress, FBType type, uint32_t size, uint32_t width, uint32_t height, uint32_t pixelsPerScanLine) :
    BaseAddress(baseAddress), Type(type), Size(size), Width(width), Height(height), PixelsPerScanLine(pixelsPerScanLine) { }