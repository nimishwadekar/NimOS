#include "Interrupts.hpp"
#include "../Display/Renderer.hpp"

#define _intr_ __attribute__((interrupt))

static void InitializeIDTEntry(const uint8_t interrupt, const uint64_t handler);

_intr_ static void PageFaultHandler(ExceptionFrame *frame);

void InitializeInterrupts(void)
{
    InitializeIDTEntry(0xE, (uint64_t) PageFaultHandler);

    IDTRegister.Limit = 0x0FFF;
    IDTRegister.PhysicalAddress = (uint64_t) IDT;
    asm volatile("lidt %0" : : "m"(IDTRegister));
}

static void InitializeIDTEntry(const uint8_t interrupt, const uint64_t handler)
{
    IDTEntry entry = IDT[interrupt];
    entry.SetOffset(handler);
    entry.IST = 0;
    entry.Type_Attribute = IDT_TA_InterruptGate;
    entry.Selector = 0x08;
    entry.ZERO = 0;
    IDT[interrupt] = entry;
}

_intr_ static void PageFaultHandler(ExceptionFrame *frame)
{
    uint64_t cr2;
    asm volatile("mov %%cr2, %0" : "=r"(cr2) : );
    MainRenderer.PrintErrorf("PAGE FAULT at virtual address 0x%x, Error code = 0x%x\n", cr2, frame->ErrorCode);
    MainRenderer.PrintErrorf("CS : RIP = 0x%x : 0x%x\n", frame->CS, frame->RIP);
    while(1);
}