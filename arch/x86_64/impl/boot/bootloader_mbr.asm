[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, boot_msg
    call print_string

    call enable_a20

    ; Load 10 sectors from disk sector 1 to 0x7E00
    mov ax, 0x7E0      ; Segment for 0x7E00
    mov es, ax
    xor bx, bx

    mov ah, 0x02       ; Read sectors
    mov al, 10         ; Number of sectors
    mov ch, 0          ; Cylinder
    mov cl, 2          ; Sector 2 (LBA 1)
    mov dh, 0          ; Head
    mov dl, 0x80       ; First hard drive
    int 0x13
    jc disk_error

    mov si, loaded_msg
    call print_string

    ; Jump to stage 2 loader at 0x7E00
    jmp 0x7E00

print_string:
    mov ah, 0x0E
.print_loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .print_loop
.done:
    ret

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

disk_error:
    mov si, err_msg
    call print_string
    cli
    hlt
    jmp $

boot_msg db "MBR Bootloader: Loading Stage 2...",0
loaded_msg db "Stage 2 loaded. Jumping...",0
err_msg db "Disk read error!",0

times 510 - ($ - $$) db 0
dw 0xAA55
