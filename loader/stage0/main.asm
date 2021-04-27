[org 0x7c00]
[bits 16]

; Goal: read next 16 sectors and jump to stage1

;.....................................code.....................................

stage0_main:
	mov [BOOT_DISK], dl

	mov bp, 0x7c00
	mov sp, bp

	cli ; disable interrupts (we dont need them in the bootloader)

	mov si, startup_msg
	call pstring

	mov ah, 0x02
	mov al, 16 ; read 16 sectors
	mov ch, 0 ; cylinder 0
	mov cl, 0x02 ; read from sector 2
	mov dh, 0 ; head 0
	mov dl, [BOOT_DISK] ; drive
	mov bx, 0x8000

	int 0x13 ; DO IT

	jc boot_fail ; see if it was succesfull

	jmp 0x8000

boot_fail:
	mov bx, boot_fail_msg
	call pstring
	hlt

%include "print.asm"

;.....................................data.....................................

startup_msg: db "NextFS loader stage0 Copyright (C) 2021 Glowman554", 10, 13, 0
boot_fail_msg: db "Oh no i cant boot!", 10, 13, 0
BOOT_DISK equ 0x7d00

; Make bootable
times 510-($-$$) db 0
dw 0xaa55
