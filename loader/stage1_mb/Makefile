
SRC = entry.o main.o string.o elf.o screen.o port.o ata.o

CFLAGS = -m32 -nostdinc -g -fno-stack-protector -Iinclude -ffreestanding
ASFLAGS = -m32 -g
LDFLAGS = -melf_i386 -Tlink.ld

all: core.bin
	
core.bin: $(SRC)
	i686-linux-gnu-ld $(LDFLAGS) -o $@.elf $^
	objcopy -O binary $@.elf $@ 
	nasm main.asm -fbin -o main.bin

%.o: %.c
	@echo CC $^
	@i686-linux-gnu-gcc $(CFLAGS) -c -o $@ $^

%.o: %.S
	@echo AS $^
	@i686-linux-gnu-gcc $(ASFLAGS) -c -o $@ $^

%.o: %.asm
	@echo NASM $^
	@nasm -felf -o $@ $^

clean:
	rm *.o
	rm *.elf
	rm *.bin

.PHONY: clean