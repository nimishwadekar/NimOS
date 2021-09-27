[bits 64]

section .data
msg: db "Hello Userland!", 0

section .text
global _start
_start:
    mov rsi, msg
    mov rax, 0x02
    syscall

    mov rdi, 100
    mov rsi, 100
    mov r8, 200
    mov r9, 100
    mov r10, 0x0000FFFF
    mov rax, 0x8A
    syscall

    jmp $