global long_mode_start
extern kernel_main
extern stack_top

section .text ; Processor Instructions (TODO read-only)
bits 64
long_mode_start:
    ; Load data segment selector into data registers
    mov ax, 0x10          ; Data segment selector (from GDT)
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Reload code segment selector with a far jump
    mov ax, 0x08          ; Code segment selector (from GDT)
    push rax              ; Push the new CS
    lea rax, [rel next]   ; Push the address of the next instruction
    push rax
    retfq                 ; Far return to reload CS

next:
    ; Set up the stack
    mov rsp, stack_top
    and rsp, -16          ; Align stack before calling C handler (optional but safer)

    ; Call the kernel main function
    call kernel_main
    hlt                   ; Halt the CPU