[bits 64]

section .data
msg: db "Hello World!!", 0xA, 0xA, 0

section .text
global _start
_start:
    mov rsi, msg
    mov rax, 0x02
    syscall

    

    jmp $