global start
extern stack_top
extern long_mode_start
extern idt_flush

section .text
bits 32

start:
    ; you're now in protected mode (CS=0x08, DS=0x10)
	xchg bx, bx  ; This is Bochs's magic breakpoint

	; setup stack
	mov esp, stack_top
	mov ebp, esp

	mov al, 'F'
	out 0xE9, al

	lidt [dummy_idt_descriptor]

	; Common setup regardless of boot path
	call check_long_mode

	; Detect boot path
	cmp eax, 0x36d76289		; mb2 magic
	je .from_grub
	jmp .from_mbr

	hlt

.enter_long_mode:
	; TODO: Temp debug
	mov eax, [gdt64.pointer + 2]   ; base of GDT
	mov [0xb8010], eax
	mov dword [0xb8014], 0x4F544447 ; " GDT"

	lgdt [gdt64.pointer]
	jmp gdt64.code_segment:long_mode_start

.from_grub:
	;lidt [dummy_idt_descriptor]
	call setup_page_tables
	call enable_paging
	call .enter_long_mode

.from_mbr:
	; Setup Dummy IDT to handle any QEMU Timing issues, etc. before our 64 bit IDT is setup
	;lidt [dummy_idt_descriptor]

	;call check_cpuid ; bochs may not support, need to debug later

	call setup_page_tables
	call enable_paging
	call .enter_long_mode

dummy_idt:
    times 256 dq 0

dummy_idt_descriptor:
    dw dummy_idt_end - dummy_idt - 1
    dd dummy_idt

dummy_idt_end:

check_cpuid:
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	cmp eax, ecx
	je .no_cpuid
	ret
.no_cpuid:
	mov al, "C"
	jmp error

check_long_mode:
	mov eax, 0x80000000    ; Set the A-register to 0x80000000.
    cpuid                  ; CPU identification.
    cmp eax, 0x80000001    ; Compare the A-register with 0x80000001.
    jb .no_long_mode       ; It is less, there is no long mode.

    mov eax, 0x80000001    ; Set the A-register to 0x80000001.
    cpuid                  ; CPU identification.
    test edx, 1 << 29      ; Test if the LM-bit, which is bit 29, is set in the D-register.
    jz .no_long_mode       ; They aren't, there is no long mode.

    ret
.no_long_mode:
	mov al, "L"
	jmp error

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ; present, writable
	mov [page_table_l4], eax
	
	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	mov ecx, 0 ; counter
.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	or eax, 0b10000011 ; present, writable, huge page
	mov [page_table_l2 + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 8 ; 8 * 2 MiB = 16 MiB
	; TODO: Reducing actual size due to PIT IRQ0 alignment issue
	; cmp ecx, 512 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	ret

enable_paging:
	; pass page table location to cpu
	mov eax, page_table_l4
	mov cr3, eax

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

error:
	; print "ERR: X" where X is the error code
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

section .bss ; Un-initialized TODO read-write
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096

section .rodata ; TODO Read-only data section
gdt64:
        dq 0                     ; null descriptor
.code_segment: equ $ - gdt64
        dq 0x00AF9A000000FFFF    ; 64-bit code segment
.data_segment: equ $ - gdt64
        dq 0x00AF92000000FFFF    ; 64-bit data segment
.pointer:
        dw $ - gdt64 - 1 ; length
        dq gdt64 ; address