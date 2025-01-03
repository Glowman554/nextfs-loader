#include <stdint.h>

#define MAGIC 0xf0f0

struct nextfs_header {
	uint16_t magic;
	char label[16];

	uint16_t current_sector;
	uint16_t file_header_index;
} __attribute__((__aligned__((512))));

struct nextfs_file_header {
	char name[16];
	uint16_t start_sector;
	uint16_t length;
} __attribute__((packed));

typedef void (*sector_read)(uint8_t* buffer, uint32_t start_sector, int count, void* data);
typedef void (*sector_write)(uint8_t* buffer, uint32_t start_sector, int count, void* data);
typedef void (*screen_write)(char* fmt, ...);


struct nextfs {
	sector_read reader;
	sector_write writer;
	screen_write swriter;
	struct nextfs_header header;
	struct nextfs_file_header file_header[100];
};

void init_nextfs(struct nextfs* fs, void* data) {
	fs->reader((uint8_t*) &fs->header, 17, 512, data);
	for (int i = 0; i < 4; i++) {
		fs->reader((uint8_t*) fs->file_header + 512 * i, i + 18, 512, data);
	}
}

void uninit_nextfs(struct nextfs* fs, void* data) {
	fs->writer((uint8_t*) &fs->header, 17, 512, data);
	for (int i = 0; i < 4; i++) {
		fs->writer((uint8_t*) fs->file_header + 512 * i, i + 18, 512, data);
	}
}

int is_nextfs(struct nextfs* fs) {
	return fs->header.magic == MAGIC;
}

void format_nextfs(struct nextfs* fs, char* name) {
	fs->header.magic = MAGIC;
	fs->header.current_sector = 22;
	fs->header.file_header_index = 1;
	strcpy(fs->header.label, name);
}

void print_nextfs(struct nextfs* fs) {
	fs->swriter("[%s] current sector: %d, file header idx: %d!\n", fs->header.label, fs->header.current_sector, fs->header.file_header_index);

	for (int i = 1; i < fs->header.file_header_index; i++) {
		fs->swriter("[%s] length: %d, start sector: %d!\n", fs->file_header[i].name, fs->file_header[i].length, fs->file_header[i].start_sector);
	}
	
}

void new_file_nextfs(struct nextfs* fs, void* data, char* name, uint8_t* b, int size) {
	memset(fs->file_header[fs->header.file_header_index].name, 0, sizeof(fs->file_header[fs->header.file_header_index].name));
	strcpy(fs->file_header[fs->header.file_header_index].name, name);
	fs->file_header[fs->header.file_header_index].start_sector = fs->header.current_sector;
	fs->file_header[fs->header.file_header_index].length = size / 512;

	int sectors_needed = size / 512;
	for (int i = 0; i < sectors_needed + 1; i++) {
		fs->writer(b + 512 * i, fs->header.current_sector, 512, data);
		fs->header.current_sector++;
	}
	fs->header.file_header_index++;
}

void load_file_nextfs(struct nextfs* fs, void* data, char* name, uint8_t* b) {
	for (int i = 0; i < fs->header.file_header_index; i++) {
		if(strcmp(fs->file_header[i].name, name) == 0) {

			int sectors_needed = fs->file_header[i].length;

			for (int j = 0; j < sectors_needed + 1; j++) {
				fs->reader((uint8_t*) b + 512 * i, fs->file_header[i].start_sector + j, 512, data);
			}
		}
	}
}
