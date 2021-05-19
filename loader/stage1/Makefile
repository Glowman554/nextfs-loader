all: asm

nyan:
	make -C nyanmbr

asm: nyan
	nasm -f bin main.asm -o main.bin

clean:
	rm *.bin