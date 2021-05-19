[bits 16]

%macro inc_bin 2
	SECTION .rodata
	GLOBAL %1
	GLOBAL %1_size
%1:
	incbin %2
	db 0
	%1_size: dq %1_size - %1
%endmacro

inc_bin nyan, "./nyanmbr/nyan.mbr"

do_nyan:

	mov cx, nyan
	mov dx, 0x7c00

	mov bx, 0

.loop:
	push bx
	add bx, cx
	mov ax, [bx]
	pop bx

	push bx
	add bx, dx
	mov [bx], ax
	pop bx

	inc bx
	cmp bx, word 512
	jne .loop
	
	jmp 0x7c00