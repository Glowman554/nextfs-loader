[bits 16]

;-------------------------------------------------------------
; pstring -> general string printing routine
; BX: string pointer
;-------------------------------------------------------------

pstring:
	push ax
	push bx
	push si

	mov si, bx
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

	pop si
	pop bx
	pop ax
	ret

;-------------------------------------------------------------
; pchar -> general char printing routine
; BH: char
;-------------------------------------------------------------

pchar:
	push ax

	mov ah, 0x0e
	mov al, bh
	int 0x10

	pop ax
	ret

;-------------------------------------------------------------
; phex -> general hex printing routine
; DX: hex
;-------------------------------------------------------------

phex:
	push ax
	push bx
	push cx
	push dx

	mov cx, 4
.loop:
	mov ax, dx
	and al, 0x0f
	mov bx, hex_to_ascii
	xlatb

	mov bx, cx
	mov [hex_string + bx + 1], al
	ror dx, 4

	loop .loop

	mov bx, hex_string
	call pstring

	pop dx
	pop cx
	pop bx
	pop ax

	ret

hex_string: db "0x0000", 0
hex_to_ascii: db "0123456789abcdef"


;-------------------------------------------------------------
; newline -> general newline printing routine
;-------------------------------------------------------------

newline:
	push bx

	mov bh, 10
	call pchar
	mov bh, 13
	call pchar

	pop bx
	ret