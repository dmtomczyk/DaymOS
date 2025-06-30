[BITS 16]
global stage2_start

section .text
stage2_start:
    cli
    
    mov ah, 0x0e
    mov al, 'A'
    int 0x10

    ; 1. Check for LBA (int 13h extensions) support from BIOS
    mov ax, 0x4100
    mov bx, 0x55AA
    mov dl, 0x80         ; drive number (e.g. 0x80 for HDD)
    int 0x13
    jc  .no_extensions    ; carry set → not supported
    cmp bx, 0xAA55
    jne .no_extensions    ; signature mismatch
    test cx, 1
    jz  .no_extensions    ; bit 0 clear → extensions not supported

    ; 1.5 Enable A20
    in al, 0x92
    or al, 0x02
    out 0x92, al

    ; 2. Read kernel into memory
    call read_kernel_chs

    mov ah, 0x0e
    mov al, 'C'
    int 0x10

    ; 3. Setup GDT & enter protected
    call setup_gdt

    mov ah, 0x0e
    mov al, 'D'
    int 0x10

	mov al, 'Q'
	out 0xE9, al

    mov eax, cr0
    and eax, 0x7FFFFFFF
    or eax, 1
    mov cr0, eax

	mov al, 'E'
	out 0xE9, al

    ; Far jump to flush pipeline and switch to 32-bit
    jmp 0x08:protected_start

.no_extensions:
    hlt
    jmp $
read_kernel_chs:
    pusha

    ; Set ES:BX to 0x8000:0000, which maps to 0x80000 physical
    mov ax, 0x8000  ; segment
    mov es, ax
    xor bx, bx      ; offset

    ; CHS for LBA 2048 = Cylinder 2, Head 0, Sector 33
    mov ah, 0x02        ; BIOS: Read Sectors
    mov al, 33          ; Sector count (~16 KiB kernel)
    mov ch, 1           ; Cylinder
    mov cl, 2           ; Sector (bits 0–5 = 33)
    mov dh, 0           ; Head
    mov dl, 0x80        ; boot drive

    int 0x13
    jc .read_failed

    popa
    ret

.read_failed:
    mov al, 'F'
    out 0xE9, al
    cli
    hlt
    jmp $


; ========== GDT Setup ==========
section .gdt align=8
setup_gdt:
    lgdt [gdt_descriptor]
    ret

gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF  ; Code
    dq 0x00CF92000000FFFF  ; Data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

disk_error:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
.hang:
    hlt
    jmp .hang

; ========== 32-bit Mode ==========
[BITS 32]
section .text
protected_start:
    cli

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Setup a flat, safe stack
    mov esp, 0x90000            ; or: extern stack_top → mov esp, stack_top
    and esp, 0xFFFFFFF0         ; 16-byte align (optional but best practice)

    mov al, 'W'
    out 0xE9, al

    xchg bx, bx     ; TODO: Will break into debugger

    jmp 0x08:0x80000     ; entry32.asm temporarily exists here
    
    ; control transfer failed?
    mov al, 'Z'
    out 0xE9, al

