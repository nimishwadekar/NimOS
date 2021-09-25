[bits 64]
[org 0x500000000]

jmp beginUser

msg: db "Hello World!!", 0xA, 0
inp: times 10 db 0

beginUser:
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