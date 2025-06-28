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

    ; 2. Read kernel into memory
    call read_kernel_lba

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

; Input: DL = BIOS boot drive number (e.g., 0x80)
;        ES:BX = pointer to disk_address_packet (DAP)
;        LBA 2048 = kernel sector start
;        Read 33 sectors = ~16KB kernel

read_kernel_lba:
    mov ah, 0x0e
    mov al, 'B'
    int 0x10
    
    pusha

    ; Setup Disk Address Packet (DAP) in memory
    ; Must be aligned and below 1MB (e.g., 0x8000 or lower)
    mov si, disk_address_packet

    ; DAP format:
    ; offset | size | desc
    ; -------+------+-------------------------
    ;   0    | 1    | size of DAP (10h)
    ;   1    | 1    | reserved (0)
    ;   2    | 2    | sector count to read
    ;   4    | 2    | destination offset
    ;   6    | 2    | destination segment
    ;   8    | 8    | LBA (QWORD)

    ; DAP[0] = 0x10
    mov byte [si], 0x10
    mov byte [si+1], 0x00          ; reserved
    mov word [si+2], 33            ; # of sectors (~16 KB)
    mov word [si+4], 0x0000        ; offset = 0x0000
    mov word [si+6], 0x1000        ; segment = 0x1000 → 0x10000:0 = 0x100000
    mov dword [si+8], 2048         ; LBA low (start sector)
    mov dword [si+12], 0           ; LBA high (zero for < 2TB)

    ; Point ES:BX to DAP
    mov bx, si
    mov ax, 0
    mov es, ax

    ; Set up registers
    mov ah, 0x42
    int 0x13
    jc .read_error

    popa
    ret

.read_error:
    ; You can add diagnostic code here or loop forever
    cli
    hlt
    jmp $

; ------------------------------------------------
; Data Section (place near top of stage2.asm)
disk_address_packet:
    times 16 db 0     ; must be 16 bytes total

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

    xchg bx, bx     ; Will break into debugger

    jmp 0x08:0x00100000     ; entry32.asm starts here
    
    ; control transfer failed?
    mov al, 'Z'
    out 0xE9, al

