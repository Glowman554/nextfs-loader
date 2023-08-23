[bits 16]
[org 0x8000]



loader:
    call do_e820

    call enable_a20
    
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp codeseg:protected_mode


do_e820:
	mov di, 0x500
	mov ax, 0
	mov es, ax
	xor ebx, ebx
	xor ebp, ebp

.next:
	add di, 24
	inc ebp

	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc .finish	; carry set on first call means "unsupported function"

	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne .finish

	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je .finish
	jmp .next
.finish:
	mov [0x500], ebp	; store the entry count
	ret

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

gdt_nulldesc:
    dd 0
    dd 0    
gdt_codedesc:
    dw 0xFFFF           ; Limit
    dw 0x0000           ; Base (low)
    db 0x00             ; Base (medium)
    db 10011010b        ; Flags
    db 11001111b        ; Flags + Upper Limit
    db 0x00             ; Base (high)
gdt_datadesc:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    gdt_size: 
        dw gdt_end - gdt_nulldesc - 1
        dq gdt_nulldesc

codeseg equ gdt_codedesc - gdt_nulldesc
dataseg equ gdt_datadesc - gdt_nulldesc

[bits 32]

protected_mode:

	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    jmp kernel

times 256-($-$$) DB 0

[bits 32]

kernel:
    incbin "core.bin"