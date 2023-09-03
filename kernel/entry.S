.section .text.init

.extern _main
.global _start
_start:
    cli
    
    mov %cr0, %eax
    and $~(1 << 31), %eax
    mov %eax, %cr0

    lidt (0)

    mov $0x7d000, %esp
	call _main
	jmp .