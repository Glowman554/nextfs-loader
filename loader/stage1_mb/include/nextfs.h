#pragma once

#define MAGIC 0xf0f0

struct nextfs_header {
	unsigned short magic;
	char label[16];

	unsigned short current_sector;
	unsigned short file_header_index;
} __attribute__((__aligned__((512))));

struct nextfs_file_header {
	char name[16];
	unsigned short start_sector;
	unsigned short length;
} __attribute__((packed));
