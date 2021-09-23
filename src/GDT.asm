[bits 64]

global LoadGDT
LoadGDT:
    lgdt [rdi]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov gs, ax
    mov fs, ax

    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    retfq