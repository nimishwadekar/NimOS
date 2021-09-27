#include <Display/Framebuffer.hpp>

Framebuffer::Framebuffer(uint32_t *baseAddress, FBType type, uint32_t size, int32_t width, int32_t height, int32_t pixelsPerScanLine) :
    BaseAddress(baseAddress), Type(type), Size(size), Width(width), Height(height), PixelsPerScanLine(pixelsPerScanLine) { }