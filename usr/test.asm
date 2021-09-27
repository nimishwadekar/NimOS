[bits 64]

section .data
msg: db "Hello World!!", 0xA, 0xA, 0
fname: db "anotherDirectory/src/hello.c", 0
handle: dq 0
size: dq 0

buf equ 0x500000000

section .text
global _start
_start:
    mov rsi, msg
    mov rax, 0x02
    syscall

    mov rsi, fname
    mov rdx, 0
    mov rax, 0x11
    syscall
    mov [handle], rax

    mov rdx, [handle]
    mov r10, 0
    mov r8, 2
    mov rax, 0x15
    syscall
    mov rax, 0x16
    syscall
    mov [size], rax
    mov r8, 0
    mov rax, 0x15
    syscall

    mov rsi, buf
    mov r10, 1
    mov r8, 1
    mov rax, 0x41
    syscall

    mov rdi, buf
    mov r10, [size]
    mov rax, 0x13
    syscall

    mov rsi, buf
    mov rax, 0x02
    syscall

    jmp $