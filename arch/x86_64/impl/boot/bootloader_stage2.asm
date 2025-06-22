[BITS 16]
[ORG 0x7E00]          ; Loaded at 0x7E00 by MBR bootloader

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00

    mov si, boot_msg
    call print_string

    call enable_a20

    ; Setup GDT
    lgdt [gdt_descriptor]

    ; Enable Protected Mode (CR0.PE = 1)
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:init_pm

[BITS 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov eax, PAGE_TABLE
    mov cr3, eax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Enable Long Mode (LME) in EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable Paging (PG bit)
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    jmp 0x08:kernel_entry

[BITS 64]
kernel_entry:
    mov rsp, 0x90000
    call start       ; Your 64-bit kernel entry point
    hlt

print_string:
    mov ah, 0x0E
.next_char:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .next_char
.done:
    ret

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

; ---------------------------
; GDT definitions

gdt_start:
    dq 0x0000000000000000             ; Null descriptor
    dq 0x00AF9A000000FFFF             ; Code segment: base=0, limit=4GB, execute/read, L-bit=1 (64-bit)
    dq 0x00AF92000000FFFF             ; Data segment: base=0, limit=4GB, read/write

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

gdt_end:

CODE_SEG equ 0x08
DATA_SEG equ 0x10

; ---------------------------
; Page tables aligned to 4K boundary

align 4096
PAGE_TABLE:
    dd pdpt_table
    dd 0x00000003

pdpt_table:
    dd pd_table
    dd 0x00000003

pd_table:
    dd 0x00200003        ; 2MB page + flags: present + rw + ps
    dd 0x00000000

times 4096 - ($ - PAGE_TABLE) db 0

; ---------------------------
; Bootloader message

boot_msg db "Stage 2: entering protected mode...",0