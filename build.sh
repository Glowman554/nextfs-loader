make -C loader
make -C tools
make -C kernel

./tools/fstool.elf ./loader/loader.bin ./kernel/kernel.bin:kernel

make -C loader clean
make -C tools clean
make -C kernel clean

#qemu-system-i386 -hda fs.nfs