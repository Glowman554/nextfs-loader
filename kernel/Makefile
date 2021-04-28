
KERNEL = main.o

CFLAGS = -m32 -nostdinc
LDFLAGS = -melf_i386 --static --nostdlib

all: kernel.bin
	
kernel.bin: $(KERNEL)
	ld $(LDFLAGS) -Ttext=0x0 -o $@.elf $^
	objcopy -O binary -j .text $@.elf $@ 

%.o: %.c
	@echo CC $^
	@gcc $(CFLAGS) -c -o $@ $^

clean:
	rm *.o
	rm *.elf
	rm *.bin

.PHONY: clean