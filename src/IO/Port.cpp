#include "Port.hpp"

void outb(uint16_t port, uint8_t value)
{
    asm volatile("out %%al, %%dx" : : "a"(value), "d"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile("in %%dx, %%al" : "=a"(value) : "d"(port));
    return value;
}

void outw(uint16_t port, uint16_t value)
{
    asm volatile("out %%ax, %%dx" : : "a"(value), "d"(port));
}

uint16_t inw(uint16_t port)
{
    uint16_t value;
    asm volatile("in %%dx, %%ax" : "=a"(value) : "d"(port));
    return value;
}

void io_wait(void)
{
    asm volatile("out %0, $0x80" : : "r"(0));
}