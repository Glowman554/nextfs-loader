[bits 16]

;-------------------------------------------------------------
; init_nextfs -> read all necesary sectors into ram
;-------------------------------------------------------------


init_nextfs:
	push ax
	push bx
	push cx

	mov ax, 17 ; fs header sector
	mov bx, DISK_READ_BUFFER

	call dread_sector

	cmp [DISK_READ_BUFFER], word 0xf0f0
	jne yeet ; magic is wrong we cant boot

	mov ax, [DISK_READ_BUFFER + 18]
	mov [nextfs_current_sector], ax
	mov ax, [DISK_READ_BUFFER + 20]
	mov [nextfs_file_header_idx], ax

	mov cx, 0

.read_loop:
	mov ax, 18
	add ax, cx

	mov bx, 512 ; write after fs header

	; multiplication bx = bx * cx
	push ax
	mov ax, bx
	mul cx
	mov bx, ax
	pop ax

	add bx, 512 ; add file header offset
	add bx, DISK_READ_BUFFER ; add the buffer pointer

	; we have now ax = sector, bx = address
	call dread_sector

	add cx, 1
	cmp cx, byte 4
	jne .read_loop ; read all 4 sectors with file headers

	pop cx
	pop bx
	pop ax

	ret

;-------------------------------------------------------------
; print_fs -> print all aviable files in filesystem
;-------------------------------------------------------------

print_fs:
	push ax
	push bx

	mov bx, [nextfs_file_header_idx]
	sub bx, 1

.read_loop:
	mov ax, 20 ; calculate file header offset
	mul bx

	add ax, DISK_READ_BUFFER + 512 ; add necesary offsets

	; ax now contains pointer to file header

	push bx
	mov bx, nextfs_file_s
	call pstring
	mov bx, ax
	call pstring
	call newline
	pop bx

	sub bx, 1
	cmp bx, byte 0
	jne .read_loop

	pop bx
	pop ax
	ret

;-------------------------------------------------------------
; find_file -> search all aviable files in filesystem
; CX: pointer to file header
; DX: string pointer
;-------------------------------------------------------------

find_file:
	push ax
	push bx

	mov [.buffer], dx

	mov bx, [nextfs_file_header_idx]
	sub bx, 1

.read_loop:
	mov ax, 20 ; calculate file header offset
	mul bx

	add ax, DISK_READ_BUFFER + 512 ; add necesary offsets

	; ax contains file header pointer
	; the file header also starts with a string containing the filename
	mov cx, ax
	mov dx, [.buffer]
	call strcmp ; compare string to string we are searching

	cmp ax, word 0
	je .done ; we found it yay


	sub bx, 1
	cmp bx, byte 0
	jne .read_loop ; we have still headers left to read

	mov ax, 0x3 ; we didnt find the file
	jmp yeet ; YEEEEEEEEEEEEET

.done:
	pop bx
	pop ax
	ret

.buffer: dw 0

;-------------------------------------------------------------
; load_file -> load a file at a specific address
; BX: file header pointer
; CX: buffer pointer
;-------------------------------------------------------------

load_file:
	push ax
	push bx
	push cx
	push dx

	mov ax, [bx + 18] ; lenght
	add ax, 1
	mov bx, [bx + 16] ; start sector

.read_loop:
	push ax


	mov dx, 512
	mul dx ; ax = ax * 512
	
	mov dx, ax
	sub dx, 0x200 ; we want to start with offset 0x0 not 0x200

	pop ax

	add dx, cx ; add buffer pointer 

	push ax
	push bx

	add ax, bx ; calculate sector to read
	sub ax, 1 ; we are 1 sectore ahead
	mov bx, dx
	call dread_sector

	pop bx
	pop ax

	sub ax, 1

	cmp ax, byte 0
	jne .read_loop ; we still have data left to read

	pop dx
	pop cx
	pop bx
	pop ax

	ret

nextfs_current_sector: dw 0
nextfs_file_header_idx: dw 0

nextfs_file_s: db "Found file: ", 0