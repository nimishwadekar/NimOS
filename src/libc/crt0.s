[bits 64]

section .text
global _start

DEBUG: db 'crt0'

extern _heap_initialize
extern main

; Parameters: stack top, heap base
; Stack: argc, argv[0], argv[1], ... argv[argc] == NULL
_start:
    mov rsp, rdi

    ; Initialize Heap.
    mov rdi, rsi
    mov rsi, 1
    call _heap_initialize

    ; Set up main() arguments.
    pop rdi
    mov rsi, rsp

    call main

    ; Exit syscall.
    mov rdi, rax
    mov rax, 0x23
    syscall

