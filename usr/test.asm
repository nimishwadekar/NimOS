[bits 64]

section .data
msg: db "Hello World!!", 0xA, 0
inp: times 12 db 0
len: db 0, 0

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

    mov rsi, inp
    mov rcx, 0
loop:
    cmp byte [rsi], 0
    jz endloop
    inc rsi
    inc rcx
    jmp loop

endloop:
    add cl, '0'
    mov [len], cl
    mov rsi, len
    mov rax, 0x02
    syscall

    jmp $