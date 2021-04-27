[bits 16]

pstring:
	mov bx, 0 ; page 0
.loop:
	lodsb ; store a single byte from si and store it in al
	cmp al, 0 ; EOF?
	je .done ; EOF.

	; Print char
	mov ah, 0x0E
	int 0x10
	jmp .loop

.done: ; Exit loop
	ret