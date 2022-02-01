; https://www.gnu.org/software/grub/manual/multiboot2/html_node/Header-layout.html#Header-layout
; Multiboot2 header layout
; Offset	| Type	| Field Name
; 0			| u32	| magic
; 4			| u32	| architecture
; 8			| u32	| header_length
; 12		| u32	| checksum
; 16-XX		| u32	| tags

section .multiboot_header
header_start:
	; magicnumber to indicate our use of multiboot2
	dd 0xe85250d6
	; architecture 
	dd 0 ;protected mode i386
	; header length
	dd header_end - header_start
	; checksum 32-bit unsigned value, which
	; when added to the other magic fields (i.e. ‘magic’ and ‘flags’), must have a sum of zero.
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

	;end tags
	dw 0
	dw 0
	dw 8
header_end:
