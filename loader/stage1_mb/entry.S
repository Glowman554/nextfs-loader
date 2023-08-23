.section .text.init

.extern _main
.global _start
_start:
	call _main
	jmp .
