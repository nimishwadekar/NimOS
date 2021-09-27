[bits 64]

section .data
msg: db "Hello Userland!!", 0
newl: db 0xA
dat: db 0

section .text
global _start
_start:

    mov rsi, msg
    mov rax, 0x02
    syscall
    call printNewl

    mov r15, 45
    mov [dat], byte '0'
next:
    mov rdx, 1
    mov r10, 250
    mov rax, 0x51
    syscall

    mov r10, 1
    mov rsi, dat
    mov rax, 0x01
    syscall
    call printNewl
    inc byte [dat]
    dec r15
    jnz next

    mov rsi, msg
    mov rax, 0x02
    syscall

    jmp $

printNewl:
    mov rsi, newl
    mov r10, 1
    mov rax, 0x01
    syscall
    retq