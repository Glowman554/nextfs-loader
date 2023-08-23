#include "elf.h"
#include "string.h"
#include "screen.h"

void* init_elf(void* image) {
	struct elf_header* header = image;

	if (header->magic != ELF_MAGIC) {
		puts("ELF magic mismatch\n");
		return (void*) 0;
	}


	struct elf_program_header* ph = (struct elf_program_header*) (((char*) image) + header->ph_offset);
	for (int i = 0; i < header->ph_entry_count; i++, ph++) {
		void* dest = (void*) ph->virt_addr;
		void* src = ((char*) image) + ph->offset;

		if (ph->type != 1) {
			continue;
		}

		memset(dest, 0, ph->mem_size);
		memcpy(dest, src, ph->file_size);
	}

    return (void*) header->entry;
}