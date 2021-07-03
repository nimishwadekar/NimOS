#include "Common.hpp"
#include "bootboot.h"
#include "Fonts/PSF.hpp"
#include "Display/Framebuffer.hpp"
#include "Display/Renderer.hpp"

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

extern volatile unsigned char _binary_font_psf_start;

extern void KernelStart();

void main()
{
    Framebuffer framebuffer((uint32_t*) bootboot.fb_ptr, (Framebuffer::FBType) bootboot.fb_type, 
        bootboot.fb_size, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline / 4);
    PSF1 *font = (PSF1*) &_binary_font_psf_start;
    MainRenderer = Renderer(framebuffer, font, 0xFFFFFFFF, 0x00000000);

    KernelStart();
}