#include "string.h"
#include "elf.h"
#include "screen.h"
#include "port.h"
#include "ata.h"
#include "multiboot.h"
#include "nextfs.h"

struct nextfs_header header;
struct nextfs_file_header file_header[128];

typedef struct SMAP_entry {
	unsigned long long base;
	unsigned long long length;
	unsigned int type;
	unsigned int ACPI;
} __attribute__((packed)) SMAP_entry_t;

SMAP_entry_t* memory_map = (SMAP_entry_t*) 0x500;

multiboot_info_t* mb_info = (multiboot_info_t*) 0x800000;
multiboot_mmap_t* mb_mmap = (multiboot_mmap_t*) 0x801000;

multiboot_info_t* build_mb_info(char* cmdline) {
	mb_info->mbs_mmap_length = memory_map->base * sizeof(multiboot_mmap_t);
	mb_info->mbs_mmap_addr = mb_mmap;

	for (int i = 0; i < memory_map->base; i++) {
		mb_mmap[i] = (multiboot_mmap_t) {
			.entry_size = sizeof(multiboot_mmap_t),
			.base = memory_map[i + 1].base,
			.length = memory_map[i + 1].length,
			.type = memory_map[i + 1].type
		};
	}


	mb_info->mbs_cmdline = (uint32_t) &mb_info[1];
	memcpy(&mb_info[1], cmdline, strlen(cmdline) + 1);

	return mb_info;
}

void load_file(unsigned short port, int master, struct nextfs_file_header* header, void* buffer) {
	puts("loading ");
	puts(header->name);
	puts("...\n");

	for (int i = 0; i < header->length + 1; i++) {
		read28_ata(port, master, header->start_sector + i, (unsigned char*) buffer + 512 * i);
	}
}

void* kernel_buffer = (void*) 0x80f000;
void try_nextfs(unsigned short port, int master) {
	char cmdline[512] = "--keymap=test:/keymap.mkm --init=test:/init.mex --serial";
	read28_ata(port, master, 17, (uint8_t*) &header);
	if (header.magic != MAGIC) {
		return;
	}

	for (int i = 0; i < 4; i++) {
        read28_ata(port, master, 18 + i, (unsigned char*) file_header + 512 * i);
	}

	for (int i = 1; i < header.file_header_index; i++) {
		if (strcmp(file_header[i].name, "cmdline.txt") == 0) {
			load_file(port, master, &file_header[i], cmdline);

			for (int i = 0; i < sizeof(cmdline); i++) {
				if (cmdline[i] == '$') {
					cmdline[i] = 0;
					break;
				}
			}

			puts("cmdline: ");
			puts(cmdline);
			puts("\n");
		}
	}

	for (int i = 1; i < header.file_header_index; i++) {
		if (strcmp(file_header[i].name, "kernel.elf") == 0) {
			load_file(port, master, &file_header[i], kernel_buffer);

			void* entry = init_elf(kernel_buffer);
			if (entry) {
				asm volatile ("jmp %%eax" :: "a"(entry), "b"(build_mb_info(cmdline)));
			}
		}
	}
}

void _main(void) {
	outb(0x3D4,14);
    outb(0x3D5,0x07);
    outb(0x3D4,15);
    outb(0x3D5,0xD0);

#if 1
	char* video = (char*) 0xb8000;
	for (int i = 0; i < 25 * 80; i++) {
		video[2 * i] = ' ';
		video[2 * i + 1] = 0x07;
	}
#endif

	puts("NextFS loader stage1 (MicroOS) Copyright (C) 2023-2025 Glowman554\n");

	if (test_ata(0x1F0, 1)) {
		puts("ata0 master present\n");
		try_nextfs(0x1F0, 1);
	}
	if (test_ata(0x1F0, 0)) {
		puts("ata0 slave present\n");
		try_nextfs(0x1F0, 0);
	}

	if (test_ata(0x170, 1)) {
		puts("ata1 master present\n");
		try_nextfs(0x170, 1);
	}
	if (test_ata(0x170, 0)) {
		puts("ata1 slave present\n");
		try_nextfs(0x170, 0);
	}

	puts("Failed to boot\n");

	while(1) {
		__asm__ __volatile__("hlt;");
	}
}