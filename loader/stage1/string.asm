;-------------------------------------------------------------
; strlen -> string length
; AX: (return) length
; BX: pointer to string
;-------------------------------------------------------------

strlen:
	push bx
	mov ax, 0
.loop:
	cmp [bx], byte 0
	je .exit

	inc bx
	inc ax

	jmp .loop

.exit:
	pop bx
	ret

;-------------------------------------------------------------
; strcmp -> string compare
; AX: (return) status
; CX: pointer to string 1
; DX: pointer to string 2
;-------------------------------------------------------------

strcmp:
	push bx
	push cx
	push dx

	mov [.buffer1], cx
	mov [.buffer2], dx

	mov bx, [.buffer1]
	call strlen
	mov cx, ax

	mov bx, [.buffer2]
	call strlen
	mov dx, ax

	cmp dx, cx
	jne .fail



	; the strings are at least the same size
	mov ax, cx
.loop:
	mov bx, [.buffer1]
	add bx, ax
	mov cx, [bx]

	mov bx, [.buffer2]
	add bx, ax
	mov dx, [bx]

	cmp cx, dx
	jne .fail

	sub ax, 1

	cmp ax, word 0
	jne .loop
	jmp .exit

.fail:
	pop dx
	pop cx
	pop bx

	mov ax, 1
	ret
.exit:
	pop dx
	pop cx
	pop bx
	
	mov ax, 0
	ret

.buffer1: dw 0
.buffer2: dw 0