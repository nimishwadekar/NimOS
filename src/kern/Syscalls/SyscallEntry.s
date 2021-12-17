[bits 64]

extern TaskStateSegment
extern SyscallHandler

global SyscallEntry

global STATE_RBX
STATE_RBX: dq 0
global STATE_RBP
STATE_RBP: dq 0
global STATE_RSP
STATE_RSP: dq 0
global STATE_R12
STATE_R12: dq 0
global STATE_R13
STATE_R13: dq 0
global STATE_R14
STATE_R14: dq 0
global STATE_R15
STATE_R15: dq 0

global STATE_RFLAGS
STATE_RFLAGS: dq 0

global STATE_RIP
STATE_RIP: dq 0

SyscallEntry:
    mov [STATE_RIP], rcx
    mov [STATE_RFLAGS], r11

    mov [STATE_RBX], rbx
    mov [STATE_RBP], rbp
    mov [STATE_RSP], rsp
    mov [STATE_R12], r12
    mov [STATE_R13], r13
    mov [STATE_R14], r14
    mov [STATE_R15], r15

    mov rcx, r10
    mov rsp, [TaskStateSegment + 4]
    push rax

    call SyscallHandler
    pop rax
    
    mov rsp, [STATE_RSP]
    mov rbx, [STATE_RBX]
    mov rbp, [STATE_RBP]
    mov r12, [STATE_R12]
    mov r13, [STATE_R13]
    mov r14, [STATE_R14]
    mov r15, [STATE_R15]

    mov rcx, [STATE_RIP]
    mov r11, [STATE_RFLAGS]
    o64 sysret