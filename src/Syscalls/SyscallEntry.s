[bits 64]

extern TaskStateSegment
extern SyscallHandler

global SyscallEntry

; Parameters rdi, rsi, rdx, r10, r8, r9
; Registers preserved rbx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10
STATE_RBX: dq 0
STATE_RDX: dq 0
STATE_RSI: dq 0
STATE_RDI: dq 0
STATE_RBP: dq 0
STATE_RSP: dq 0
STATE_R8:  dq 0
STATE_R9:  dq 0
STATE_R10: dq 0

STATE_RFLAGS: dq 0
STATE_RIP: dq 0

SyscallEntry:
    mov [STATE_RIP], rcx
    mov [STATE_RFLAGS], r11

    mov [STATE_RBX], rbx
    mov [STATE_RDX], rdx
    mov [STATE_RSI], rsi
    mov [STATE_RDI], rdi
    mov [STATE_RBP], rbp
    mov [STATE_RSP], rsp
    mov [STATE_R8], r8
    mov [STATE_R9], r9
    mov [STATE_R10], r10

    mov rcx, r10
    mov rsp, [TaskStateSegment + 4]
    push rax

    call SyscallHandler

    pop rax
    mov rbx, [STATE_RBX]
    mov rdx, [STATE_RDX]
    mov rsi, [STATE_RSI]
    mov rdi, [STATE_RDI]
    mov rbp, [STATE_RBP]
    mov rsp, [STATE_RSP]
    mov r8, [STATE_R8]
    mov r9, [STATE_R9]
    mov r10, [STATE_R10]

    mov rcx, [STATE_RIP]
    mov r11, [STATE_RFLAGS]

    o64 sysret