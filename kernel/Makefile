SRC = entry.o main.o com1.o io.o

CFLAGS = -m32 -nostdinc -g -fno-stack-protector -Iinclude -ffreestanding -fpie
ASFLAGS = -m32 -g -fpie
LDFLAGS = -melf_i386 -Tlink.ld -pie

all: kernel.bin
	
kernel.bin: $(SRC)
	i686-linux-gnu-ld $(LDFLAGS) -o $@.elf $^
	i686-linux-gnu-objcopy -O binary $@.elf $@ 

%.o: %.c
	@echo CC $^
	@i686-linux-gnu-gcc $(CFLAGS) -c -o $@ $^

%.o: %.S
	@echo AS $^
	@i686-linux-gnu-gcc $(ASFLAGS) -c -o $@ $^

%.o: %.asm
	@echo NASM $^
	@nasm -felf -o $@ $^

compile_flags.txt:
	deno run -A ../compile_flags.ts $(CFLAGS) > compile_flags.txt

clean:
	rm *.o
	rm *.elf
	rm *.bin
	rm compile_flags.txt

.PHONY: clean