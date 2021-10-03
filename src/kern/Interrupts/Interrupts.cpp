#include <Display/Renderer.hpp>
#include <IO/Keyboard.hpp>
#include <IO/PIT.hpp>
#include <IO/Port.hpp>
#include <Interrupts/Interrupts.hpp>

#define _intr_ __attribute__((interrupt))

static const char *InterruptMessages[]
{
    // Exceptions
    "Divide by zero error", // 0
    "Debug trap", // 1
    "Non maskable interrupt", // 2
    "Breakpoint trap", // 3
    "Overflow trap", // 4
    "Bound range exceeded exception", // 5
    "Invalid opcode exception", // 6
    "Device not available exception", // 7
    "DOUBLE FAULT ABORT", // 8
    "Coprocessor segment overrun exception", // 9
    "Invalid TSS exception", // A
    "Segment not present exception", // B
    "Stack-segement fault", // C
    "General protection fault", // D
    "Page fault", // E
    "Reserved 0xF", // F
    "Floating point exception", // 10
    "Alignment check exception", // 11
    "MACHINE CHECK ABORT", // 12
    "SIMD floating point exception", // 13
    "Virtualization exception", // 14
    "Reserved 0x15", // 15
    "Reserved 0x16", // 16
    "Reserved 0x17", // 17
    "Reserved 0x18", // 18
    "Reserved 0x19", // 19
    "Reserved 0x1A", // 1A
    "Reserved 0x1B", // 1B
    "Reserved 0x1C", // 1C
    "Reserved 0x1D", // 1D
    "Security exception", // 1E
    "Reserved 0x1F", // 1F

    // PIC Interrupts
    "Timer interrupt", // 20        
    "Keyboard interrupt", // 21        
    "PIC 2 interrupt", // 22        
    "COM 2 interrupt", // 23        
    "COM 1 interrupt", // 24
    "LPT 2 interrupt", // 25        
    "Floppy disk interrupt", // 26        
    "LPT 1 interrupt", // 27        
    "Real time clock interrupt", // 28        
    "General I/O interrupt 0", // 29        
    "General I/O interrupt 1", // 2a        
    "General I/O interrupt 2", // 2b        
    "General I/O interrupt 3", // 2c        
    "Coprocessor interrupt", // 2d        
    "IDE Bus interrupt 0", // 2e        
    "IDE Bus interrupt 1", // 2f
};

static void InitializeIDTEntry(const uint8_t interrupt, const uint64_t handler);

#pragma region HANDLER_HEADERS
_intr_ static void IntHandler0x0(InterruptFrame *frame);
_intr_ static void IntHandler0x1(InterruptFrame *frame);
_intr_ static void IntHandler0x2(InterruptFrame *frame);
_intr_ static void IntHandler0x3(InterruptFrame *frame);
_intr_ static void IntHandler0x4(InterruptFrame *frame);
_intr_ static void IntHandler0x5(InterruptFrame *frame);
_intr_ static void IntHandler0x6(InterruptFrame *frame);
_intr_ static void IntHandler0x7(InterruptFrame *frame);
_intr_ static void IntHandler0x8(ExceptionFrame *frame);
_intr_ static void IntHandler0x9(InterruptFrame *frame);
_intr_ static void IntHandler0xa(ExceptionFrame *frame);
_intr_ static void IntHandler0xb(ExceptionFrame *frame);
_intr_ static void IntHandler0xc(ExceptionFrame *frame);
_intr_ static void IntHandler0xd(ExceptionFrame *frame);
_intr_ static void IntHandler0xe(ExceptionFrame *frame);
_intr_ static void IntHandler0xf(InterruptFrame *frame);
_intr_ static void IntHandler0x10(InterruptFrame *frame);
_intr_ static void IntHandler0x11(ExceptionFrame *frame);
_intr_ static void IntHandler0x12(InterruptFrame *frame);
_intr_ static void IntHandler0x13(InterruptFrame *frame);
_intr_ static void IntHandler0x14(InterruptFrame *frame);
_intr_ static void IntHandler0x15(InterruptFrame *frame);
_intr_ static void IntHandler0x16(InterruptFrame *frame);
_intr_ static void IntHandler0x17(InterruptFrame *frame);
_intr_ static void IntHandler0x18(InterruptFrame *frame);
_intr_ static void IntHandler0x19(InterruptFrame *frame);
_intr_ static void IntHandler0x1a(InterruptFrame *frame);
_intr_ static void IntHandler0x1b(InterruptFrame *frame);
_intr_ static void IntHandler0x1c(InterruptFrame *frame);
_intr_ static void IntHandler0x1d(InterruptFrame *frame);
_intr_ static void IntHandler0x1e(ExceptionFrame *frame);
_intr_ static void IntHandler0x1f(InterruptFrame *frame);
_intr_ static void IntHandler0x20(InterruptFrame *frame);
_intr_ static void IntHandler0x21(InterruptFrame *frame);
_intr_ static void IntHandler0x22(InterruptFrame *frame);
_intr_ static void IntHandler0x23(InterruptFrame *frame);
_intr_ static void IntHandler0x24(InterruptFrame *frame);
_intr_ static void IntHandler0x25(InterruptFrame *frame);
_intr_ static void IntHandler0x26(InterruptFrame *frame);
_intr_ static void IntHandler0x27(InterruptFrame *frame);
_intr_ static void IntHandler0x28(InterruptFrame *frame);
_intr_ static void IntHandler0x29(InterruptFrame *frame);
_intr_ static void IntHandler0x2a(InterruptFrame *frame);
_intr_ static void IntHandler0x2b(InterruptFrame *frame);
_intr_ static void IntHandler0x2c(InterruptFrame *frame);
_intr_ static void IntHandler0x2d(InterruptFrame *frame);
_intr_ static void IntHandler0x2e(InterruptFrame *frame);
_intr_ static void IntHandler0x2f(InterruptFrame *frame);
#pragma endregion

/*
Interrupts = 
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x9, 0xf, 
    0x10, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1f, 
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f

Exceptions = 0x8, 0xa, 0xb, 0xc, 0xd, 0x11, 0x1e
*/

static void RemapPIC(void);

void InitializeInterrupts(void)
{
    RemapPIC();
    PICUnmask(1); // Unmask Keyboard interrupt.
    PICUnmask(0); // Unmask PIT interrupt.

    #pragma region HandlerInitialization
    InitializeIDTEntry(0x0, (uint64_t) IntHandler0x0);
    InitializeIDTEntry(0x1, (uint64_t) IntHandler0x1);
    InitializeIDTEntry(0x2, (uint64_t) IntHandler0x2);
    InitializeIDTEntry(0x3, (uint64_t) IntHandler0x3);
    InitializeIDTEntry(0x4, (uint64_t) IntHandler0x4);
    InitializeIDTEntry(0x5, (uint64_t) IntHandler0x5);
    InitializeIDTEntry(0x6, (uint64_t) IntHandler0x6);
    InitializeIDTEntry(0x7, (uint64_t) IntHandler0x7);
    InitializeIDTEntry(0x8, (uint64_t) IntHandler0x8);
    InitializeIDTEntry(0x9, (uint64_t) IntHandler0x9);
    InitializeIDTEntry(0xa, (uint64_t) IntHandler0xa);
    InitializeIDTEntry(0xb, (uint64_t) IntHandler0xb);
    InitializeIDTEntry(0xc, (uint64_t) IntHandler0xc);
    InitializeIDTEntry(0xd, (uint64_t) IntHandler0xd);
    InitializeIDTEntry(0xe, (uint64_t) IntHandler0xe);
    InitializeIDTEntry(0xf, (uint64_t) IntHandler0xf);
    InitializeIDTEntry(0x10, (uint64_t) IntHandler0x10);
    InitializeIDTEntry(0x11, (uint64_t) IntHandler0x11);
    InitializeIDTEntry(0x12, (uint64_t) IntHandler0x12);
    InitializeIDTEntry(0x13, (uint64_t) IntHandler0x13);
    InitializeIDTEntry(0x14, (uint64_t) IntHandler0x14);
    InitializeIDTEntry(0x15, (uint64_t) IntHandler0x15);
    InitializeIDTEntry(0x16, (uint64_t) IntHandler0x16);
    InitializeIDTEntry(0x17, (uint64_t) IntHandler0x17);
    InitializeIDTEntry(0x18, (uint64_t) IntHandler0x18);
    InitializeIDTEntry(0x19, (uint64_t) IntHandler0x19);
    InitializeIDTEntry(0x1a, (uint64_t) IntHandler0x1a);
    InitializeIDTEntry(0x1b, (uint64_t) IntHandler0x1b);
    InitializeIDTEntry(0x1c, (uint64_t) IntHandler0x1c);
    InitializeIDTEntry(0x1d, (uint64_t) IntHandler0x1d);
    InitializeIDTEntry(0x1e, (uint64_t) IntHandler0x1e);
    InitializeIDTEntry(0x1f, (uint64_t) IntHandler0x1f);
    InitializeIDTEntry(0x20, (uint64_t) IntHandler0x20);
    InitializeIDTEntry(0x21, (uint64_t) IntHandler0x21);
    InitializeIDTEntry(0x22, (uint64_t) IntHandler0x22);
    InitializeIDTEntry(0x23, (uint64_t) IntHandler0x23);
    InitializeIDTEntry(0x24, (uint64_t) IntHandler0x24);
    InitializeIDTEntry(0x25, (uint64_t) IntHandler0x25);
    InitializeIDTEntry(0x26, (uint64_t) IntHandler0x26);
    InitializeIDTEntry(0x27, (uint64_t) IntHandler0x27);
    InitializeIDTEntry(0x28, (uint64_t) IntHandler0x28);
    InitializeIDTEntry(0x29, (uint64_t) IntHandler0x29);
    InitializeIDTEntry(0x2a, (uint64_t) IntHandler0x2a);
    InitializeIDTEntry(0x2b, (uint64_t) IntHandler0x2b);
    InitializeIDTEntry(0x2c, (uint64_t) IntHandler0x2c);
    InitializeIDTEntry(0x2d, (uint64_t) IntHandler0x2d);
    InitializeIDTEntry(0x2e, (uint64_t) IntHandler0x2e);
    InitializeIDTEntry(0x2f, (uint64_t) IntHandler0x2f);
    #pragma endregion

    IDTRegister.Limit = 0x0FFF;
    IDTRegister.PhysicalAddress = (uint64_t) IDT;
    asm volatile("lidt %0" : : "m"(IDTRegister));

    asm volatile("sti");
}

static void RemapPIC(void)
{
    // Saving masks
    /* uint8_t mask1, mask2;
    mask1 = inb(PIC1_DATA);
    io_wait();
    mask2 = inb(PIC2_DATA);
    io_wait(); */

    // ICW 1
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // ICW 2 - Interrupt numbers
    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    // ICW 3 - Cascading PIC chips
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    // ICW 4 - 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Mask all PIC interrupts
    outb(PIC1_DATA, 0b11111111);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();
}

void PICUnmask(uint8_t interrupt)
{
    uint8_t mask;
    uint16_t port;
    
    if(interrupt < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        interrupt -= 8;
    }
    mask = inb(port);
    mask &= ~(1 << interrupt);
    outb(port, mask);
}

void PICMask(uint8_t interrupt)
{
    uint8_t mask;
    uint16_t port;
    
    if(interrupt < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        interrupt -= 8;
    }
    mask = inb(port);
    mask |= (1 << interrupt);
    outb(port, mask);
}

void PICEndOfInterrupt(const uint8_t interrupt)
{
    if(interrupt >= 8) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
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

// Called by exception handlers to exit current process.
extern void ProcessException();

#pragma region HANDLERS
_intr_ static void IntHandler0x0(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x0]);
    ProcessException();
}

_intr_ static void IntHandler0x1(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x1]);
	while(true);
}

_intr_ static void IntHandler0x2(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x2]);
	while(true);
}

_intr_ static void IntHandler0x3(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x3]);
	while(true);
}

_intr_ static void IntHandler0x4(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x4]);
	while(true);
}

_intr_ static void IntHandler0x5(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x5]);
    ProcessException();
}

_intr_ static void IntHandler0x6(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x6]);
    ProcessException();
}

_intr_ static void IntHandler0x7(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x7]);
    ProcessException();
}

_intr_ static void IntHandler0x8(ExceptionFrame *frame)
{
    printf("%s\nError Code = 0x%x", InterruptMessages[0x8], frame->ErrorCode);
	ProcessException();
}

_intr_ static void IntHandler0x9(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x9]);
	ProcessException();
}

_intr_ static void IntHandler0xa(ExceptionFrame *frame)
{
    printf("%s\nError Code = 0x%x", InterruptMessages[0xa], frame->ErrorCode);
	ProcessException();
}

_intr_ static void IntHandler0xb(ExceptionFrame *frame)
{
    printf("%s\nError Code = 0x%x", InterruptMessages[0xb], frame->ErrorCode);
	ProcessException();
}

_intr_ static void IntHandler0xc(ExceptionFrame *frame)
{
    printf("%s\nError Code = 0x%x", InterruptMessages[0xc], frame->ErrorCode);
	ProcessException();
}

_intr_ static void IntHandler0xd(ExceptionFrame *frame)
{
    printf("%s\nError Code = 0x%x", InterruptMessages[0xd], frame->ErrorCode);
	ProcessException();
}

_intr_ static void IntHandler0xe(ExceptionFrame *frame)
{
    uint64_t cr2;
    asm volatile("mov %%cr2, %0" : "=r"(cr2) : );
    
    uint64_t error = frame->ErrorCode;
    const char *flags[5];
    flags[0] = (error & 0b00001) ? "Present" : "Non-present";
    flags[1] = (error & 0b00010) ? "Write" : "Read";
    flags[2] = (error & 0b00100) ? "User" : "";
    flags[3] = (error & 0b01000) ? "Reserved-write" : "";
    flags[4] = (error & 0b10000) ? "Instruction-fetch" : "";

    printf("PAGE FAULT at virtual address 0x%x : %s %s %s %s %s\n", cr2,
        flags[0], flags[1], flags[2], flags[3], flags[4]);
    printf("CS : RIP = 0x%x : 0x%x\n", frame->CS, frame->RIP);
    printf("err: %x, cs: %x, ip: %x, rsp: %x, flags: %x\n", frame->ErrorCode, frame->CS, frame->RIP, frame->RSP, frame->RFlags);
    ProcessException();
}

_intr_ static void IntHandler0xf(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0xf]);
	while(true);
}

_intr_ static void IntHandler0x10(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x10]);
	ProcessException();
}

_intr_ static void IntHandler0x11(ExceptionFrame *frame)
{
    printf("%s\nError Code = 0x%x", InterruptMessages[0x11], frame->ErrorCode);
	ProcessException();
}

_intr_ static void IntHandler0x12(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x12]);
	ProcessException();
}

_intr_ static void IntHandler0x13(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x13]);
	ProcessException();
}

_intr_ static void IntHandler0x14(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x14]);
	ProcessException();
}

_intr_ static void IntHandler0x15(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x15]);
	while(true);
}

_intr_ static void IntHandler0x16(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x16]);
	while(true);
}

_intr_ static void IntHandler0x17(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x17]);
	while(true);
}

_intr_ static void IntHandler0x18(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x18]);
	while(true);
}

_intr_ static void IntHandler0x19(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x19]);
	while(true);
}

_intr_ static void IntHandler0x1a(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x1a]);
	while(true);
}

_intr_ static void IntHandler0x1b(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x1b]);
	while(true);
}

_intr_ static void IntHandler0x1c(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x1c]);
	while(true);
}

_intr_ static void IntHandler0x1d(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x1d]);
	while(true);
}

_intr_ static void IntHandler0x1e(ExceptionFrame *frame)
{
    printf("%s\nError Code = 0x%x", InterruptMessages[0x1e], frame->ErrorCode);
	ProcessException();
}

_intr_ static void IntHandler0x1f(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x1f]);
	while(true);
}

_intr_ static void IntHandler0x20(InterruptFrame *frame)
{
    PIT::Tick();
    PICEndOfInterrupt(0);
}

_intr_ static void IntHandler0x21(InterruptFrame *frame)
{
    const uint8_t scanCode = inb(0x60);
    HandleKeyboard(scanCode);
    PICEndOfInterrupt(1);
}

_intr_ static void IntHandler0x22(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x22]);
	while(true);
}

_intr_ static void IntHandler0x23(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x23]);
	while(true);
}

_intr_ static void IntHandler0x24(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x24]);
	while(true);
}

_intr_ static void IntHandler0x25(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x25]);
	while(true);
}

_intr_ static void IntHandler0x26(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x26]);
	while(true);
}

_intr_ static void IntHandler0x27(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x27]);
	while(true);
}

_intr_ static void IntHandler0x28(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x28]);
	while(true);
}

_intr_ static void IntHandler0x29(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x29]);
	while(true);
}

_intr_ static void IntHandler0x2a(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x2a]);
	while(true);
}

_intr_ static void IntHandler0x2b(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x2b]);
	while(true);
}

_intr_ static void IntHandler0x2c(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x2c]);
	while(true);
}

_intr_ static void IntHandler0x2d(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x2d]);
	while(true);
}

_intr_ static void IntHandler0x2e(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x2e]);
	while(true);
}

_intr_ static void IntHandler0x2f(InterruptFrame *frame)
{
    printf("%s\n", InterruptMessages[0x2f]);
	while(true);
}
#pragma endregion