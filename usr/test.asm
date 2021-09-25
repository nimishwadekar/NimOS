[bits 64]

somedata dw 0xFACE

beginUser:
    mov ax, [somedata]
    syscall
    syscall
    jmp $