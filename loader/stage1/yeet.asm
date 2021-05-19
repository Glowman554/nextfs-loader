[bits 16]

;-------------------------------------------------------------
; yeet -> general error function (NO RETURN)
; AX: error code
;
; aviabel error codes:
;	0x1 -> Disk read error
;	0x2 -> nextfs magic error
;	0x3 -> file not found
;-------------------------------------------------------------

yeet:
	mov bx, .yeet_msg
	call pstring

	cmp ax, word 0x1
	je .yeet_disk

	cmp ax, word 0x2
	je .yeet_magic

	cmp ax, word 0x3
	je .yeet_file

	jmp .yeet_unknown


.yeet_disk:
	mov bx, .yeet_msg_disk_err
	call pstring
	jmp .yeet_done

.yeet_magic:
	mov bx, .yeet_msg_magic_err
	call pstring
	jmp .yeet_done

.yeet_file:
	mov bx, .yeet_msg_file_err
	call pstring
	jmp .yeet_done

.yeet_unknown:
	mov bx, .yeet_msg_unknown_err
	call pstring
	jmp .yeet_done

.yeet_done:
	mov bx, .yeet_nyan_msg
	call pstring
	
	mov ax, 0
	mov ah, 10h
	int 0x16

	jmp do_nyan
	hlt

.yeet_msg: db "Something is realy wrong: ", 0
.yeet_msg_disk_err: db "Disk read failed!",10, 13, 0
.yeet_msg_magic_err: db "NextFS magic invalid!",10, 13, 0
.yeet_msg_file_err: db "File not found!",10, 13, 0
.yeet_msg_unknown_err: db "Unknown error!", 10, 13, 0

.yeet_nyan_msg: db 10, 13, "Press any key to start nyan cat uwu!", 10, 13, 0