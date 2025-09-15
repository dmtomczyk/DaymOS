[BITS 16]
[ORG 0x7C00]

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00

    mov ah, 0x0e
    mov al, 'D'
    int 0x10
    mov al, 'L'
    int 0x10
    mov al, ' '
    int 0x10

    ; Print upper nibble of DL
    mov al, dl
    shr al, 4
    add al, '0'
    cmp al, '9'
    jbe .ok1
    add al, 7
    .ok1:
    int 0x10

    ; Print lower nibble of DL
    mov al, dl
    and al, 0x0F
    add al, '0'
    cmp al, '9'
    jbe .ok2
    add al, 7
    .ok2:
    int 0x10

    ;jmp $

    ; Mask all IRQs on master and slave PIC until ready to unmask later in 64bit
    mov al, 0xFF
    out 0xA1, al ; Slave PIC
    out 0x21, al ; Master PIC
    
    mov ah, 0x0e
    mov al, '1'
    int 0x10

    ; Set ES:BX = 0x0800:0000 = 0x8000
    mov ax, 0x0800
    mov es, ax
    xor bx, bx

    mov ah, 0x0e
    mov al, '2'
    int 0x10

    ; Reset Disk System
    mov ah, 0x00
    mov dl, 0x00
    int 0x13

    mov ah, 0x0e
    mov al, '3'
    int 0x10

    ; LBA = (cylinder × heads + head) × sectors + (sector - 1)
    ; Load 1 sector from CHS = (cyl=0, head=1, sector=1)
    mov ah, 0x02        ; BIOS: Read sectors
    mov al, 0x01           ; Read 1 sector from CHS (0, 1, 1) and load it to ES:BX
    mov ch, 0x00           ; cylinder #
    mov dh, 0x00           ; head #
    mov cl, 0x02           ; sector # (start at 2 to skip MBR)
    mov dl, 0x80        ; drive # (first HDD)
    int 0x13
    jc disk_error
    
    mov ah, 0x0e
    mov al, '4'
    int 0x10

    jmp 0x0800:0000

; Disk Address Packet for LBA read
; Must be exactly 16 bytes
disk_packet:
    db 0x10        ; size of packet
    db 0x00        ; reserved
    dw 0x0001      ; sectors to read
    dw 0x0000      ; offset (low word of dest)
    dw 0x0800      ; segment (0x8000 = 0x80000 linear)
    dq 0x0000000000000040  ; LBA = 64

disk_error:
    hlt
    jmp $

;disk_error:
;    mov si, msg_fail
;    call print_string
;    jmp $

print_string:
    mov ah, 0x0E
.next:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .next
.done:
    ret

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

msg_fail: db "Disk read failed", 0

times 510 - ($ - $$) db 0
dw 0xAA55