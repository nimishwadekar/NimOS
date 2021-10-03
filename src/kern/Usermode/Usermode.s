[bits 64]

extern TaskStateSegment

; Used by the kernel for the first jump into user mode.
; Parameters: Syscall entry, Stack top, Heap base, Program entry.
global JumpToUserMode
JumpToUserMode:
    ; Enable syscalls
    cli
    mov r8, rdx ; Heap base
    mov r9, rcx ; Program entry
    mov rbp, rsi ; Stack top

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

    ; No flag masks set.

    ; Set Kernel stack pointer in TSS
    mov [TaskStateSegment + 4], rsp

    ; Set up state for return to user program.
    mov rcx, r9
    mov r11, 0x202 ; correct flags
    mov rsp, rbp

    ; Set up main() arguments on stack.
    sub rsp, 16
    mov qword [rsp], 0
    mov qword [rsp + 8], 0

    mov rdi, rsp ; CRT0 argument stack top
    mov rsi, r8 ; CRT0 argument heap address

    o64 sysret


; Used by the process system calls.
; main() arguments need to be set up before calling this function.
; Parameters: Stack top, Heap base, Program entry.
global JumpToUserAddress_Syscall
JumpToUserAddress_Syscall:
    mov rcx, rdx
    mov r11, 0x202

    o64 sysret