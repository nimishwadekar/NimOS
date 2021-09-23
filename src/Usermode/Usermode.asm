[bits 64]

global JumpToUserMode
extern BeginUserMode

JumpToUserMode:
    ; Enable syscalls
    cli
    mov rbx, rdx
    mov rbp, rsi

    ; Set Syscall entry point
    mov eax, edi
    mov rdx, rdi
    shr rdx, 32
    mov ecx, 0xC0000082
    wrmsr

    ; Enable Syscall bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1
    wrmsr

    ; Set segment bases
    mov ecx, 0xC0000081
    rdmsr
    mov edx, 0x00180008
    wrmsr

    ; Set flag masks

    ; Set up state for return
    mov rcx, rbx
    mov r11, 0x202 ; correct flags
    mov rsp, rbp
    o64 sysret