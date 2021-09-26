[bits 64]

section .data
msg: db "Hello World!!", 0xA, 0
inp: times 10 db 0

section .text
global _start
_start:
    mov rsi, msg
    mov rax, 0x02
    syscall

    mov rdi, inp
    mov rax, 0x04
    syscall

    mov rsi, inp
    mov rax, 0x02
    syscall

    jmp $