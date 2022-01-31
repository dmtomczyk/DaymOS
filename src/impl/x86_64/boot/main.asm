global start

section .text
bits 32
start: 
	; print 'OK'
	mov dword [0xb80000], 0x2f4b2f4f
	hlt
