#include "Syscall.hpp"
#include "../Display/Renderer.hpp"

void Syscall(void)
{
    uint64_t rcx;
    asm volatile("movq %%rcx, %0" : "=r"(rcx));
    printf("System Call successful.\n");
    asm volatile("movq %%rax, %%rcx" : : "a"(rcx));
    asm volatile("sysretq");
}