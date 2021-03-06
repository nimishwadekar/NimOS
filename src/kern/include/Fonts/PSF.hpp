#pragma once

#include <stdint.h>

struct PSF1
{
    uint8_t Magic[2];
    uint8_t Mode;
    uint8_t BytesPerGlyph;

    // The first byte of the glyphs array.
    uint8_t Glyphs;
} __attribute__((packed));

struct PSF2
{
    uint8_t Magic[4];
    uint32_t Version;
    uint32_t HeaderSize;
    uint32_t Flags;
    uint32_t GlyphCount;
    uint32_t BytesPerGlyph;
    int32_t Height;
    int32_t Width;

    // The first byte of the glyphs array.
    uint8_t Glyphs;
} __attribute__((packed));