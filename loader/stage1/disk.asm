[bits 16]

;-------------------------------------------------------------
; dapack -> data section for int 0x13 disk read
;-------------------------------------------------------------

dapack:
	db 0x10		; size
	db 0x0		; unused should be 0
blkcnt:
	dw 0x1		; number of sectors to read
db_addr:
	dw 0x0		; addres of read buffer
	dw 0x0		; segment 0
db_lba:
	dd 0x0		; logical block address
	dd 0x0		; more storage bytes only for big lba's ( > 4 bytes )

;-------------------------------------------------------------
; dread_sector -> read 1 sector from logical block address
; AX: lba
; BX: buffer address
;-------------------------------------------------------------

dread_sector:
	push ax
	push bx
	push dx
	push si

	mov [db_lba], word ax
	mov [db_addr], word bx

	;mov dx, ax
	;call phex
	;call newline
	;mov dx, bx
	;call phex
	;call newline

	mov si, dapack
	mov ah, 0x42
	mov dl, [0x7d00]
	int 0x13
	jc .dread_err

	pop si
	pop dx
	pop bx
	pop ax

	ret

.dread_err:
	mov ax, 0x1
	jmp yeet