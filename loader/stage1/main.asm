[org 0x8000]
[bits 16]

;.....................................code.....................................

stage1_main:
	mov bx, boot_drive_string
	call pstring
	mov dx, [0x7d00]
	call phex
	call newline

	call init_nextfs
	call print_fs

	mov dx, file_to_load
	call find_file

	mov bx, cx
	mov cx, 0x0 ; load kernel at 0x0
	call load_file

	call enable_a20
	cli ; disabel interrupts
	lgdt [gdt_descriptor]

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp codeseg:protected_mode ; lets go 32 bit protected mode !!!!!

enable_a20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

[bits 32]

protected_mode:

	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	jmp 0x0 ; jump to the kernel :)

%include "print.asm"
%include "disk.asm"
%include "yeet.asm"
%include "nextfs.asm"
%include "string.asm"
%include "gdt.asm"
%include "nyan.asm"

;.....................................data.....................................

boot_drive_string: db "Boot disk: ", 0

file_to_load: db "kernel", 0

BOOT_DISK: db 0

times 8192-($-$$) db 0

DISK_READ_BUFFER equ $ ; we load the disk stuff here