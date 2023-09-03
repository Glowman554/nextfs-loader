[org 0x8000]
[bits 32]

mov ecx, 80 * 25 + 2

.clear:
    mov [0xb8000 + ecx], byte 0
    loop .clear

ret