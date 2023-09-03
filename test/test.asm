[org 0x8000]
[bits 32]

push eax
push ebx
push edx

mov bl, 27
call transmit
mov bl, '['
call transmit
mov bl, 'H'
call transmit

mov bl, 27
call transmit
mov bl, '['
call transmit
mov bl, 'J'
call transmit

mov bl, 'H'
call transmit
mov bl, 'e'
call transmit
mov bl, 'l'
call transmit
mov bl, 'l'
call transmit
mov bl, 'o'
call transmit
mov bl, 10
call transmit

pop edx
pop ebx
pop eax

ret

transmit:
.loop:
    mov dx, 0x3f8 + 5
    in al, dx
    or al, 0x20
    jz .loop

    mov dx, 0x3f8
    mov al, bl
    out dx, al
    ret
