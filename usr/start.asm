[bits 64]

section .text
global print
global _start
extern main

_start:
    call main

    jmp $

print:
    mov rsi, rdi
    mov rax, 0x02
    syscall
    retq