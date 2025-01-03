#include <stdio.h>
#include <string.h>
#include "nextfs.c"

FILE* fp;
struct nextfs fs;

void r(uint8_t* b, uint32_t start_sector, int count, void* data) {
	// printf("file -> %d\n", start_sector);
	FILE* f = (FILE*) data;
	fseek(f, start_sector * 512, SEEK_SET);
	fread(b, 1, count, f);
}

void w(uint8_t* b, uint32_t start_sector, int count, void* data) {
	// printf("file <- %d\n", start_sector);
	FILE* f = (FILE*) data;
	fseek(f, start_sector * 512, SEEK_SET);
	fwrite(b, 1, count, f);
	fflush(f);
}

uint8_t buffer[1024 * 64 * 128];

void insert(char* file, char* internal_file) {
	//uint8_t buffer[1024*64];

	FILE* data = fopen(file, "rb");
	if (data == NULL) {
		printf("Error while opening the file.\n");
		return;
	}
	fseek(data, 0, SEEK_END);
	int sz = ftell(data);
	printf("File is %d bytes big!\n", sz);
	fseek(data, 0, SEEK_SET);
	fread(buffer, 1, sz, data);
	fclose(data);
	new_file_nextfs(&fs, fp, internal_file, buffer, sz);
}

void write_bootloader(char* file) {
	FILE* loader = fopen(file, "rb");

	if (loader == NULL) {
		printf("Error while opening the file.\n");
		return;
	}
	
	fseek(loader, 0, SEEK_END);
	int sz = ftell(loader);
	fseek(loader, 0, SEEK_SET);	
	fread(buffer, 1, sz, loader);
	fseek(fp, 0, SEEK_SET);
	fwrite(buffer, 1, sz, fp);
	fflush(fp);
}

int main(int argc, char* argv[]) {

	if(argc < 2) {
		printf("Use %s <bootloader> <file:name> ...\n", argv[0]);
		return -1;
	}

	fp = fopen("fs.nfs", "wb");

	fs.reader = r;
	fs.writer = w;
	fs.swriter = (screen_write) printf;

	init_nextfs(&fs, fp);
	format_nextfs(&fs, "test");

	write_bootloader(argv[1]);

	for (int i = 0; i < argc - 2; i++) {
		char* file = strtok(argv[i + 2], ":");
		char* file_internal = strtok(NULL, ":");

		printf("%s -> %s\n", file, file_internal);
		insert(file, file_internal);
	}
	

	print_nextfs(&fs);
	uninit_nextfs(&fs, fp);

	// write bootloader



	printf("Header size: %ld\n", sizeof(struct nextfs_file_header));
}