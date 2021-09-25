[bits 64]
[org 0x500000000]

jmp beginUser

msg: db "Hello World!!", 0

beginUser:
    mov rsi, msg
    mov rax, 0x02
    syscall

    jmp $