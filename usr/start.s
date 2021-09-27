[bits 64]

section .text
global _start
extern main

_start:
    call main
    jmp $