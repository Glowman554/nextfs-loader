#include "ata.h"
#include "port.h"
#include "screen.h"

#define DATA_PORT_OFFSET 0x0
#define ERROR_PORT_OFFSET 0x1
#define SECTOR_COUNT_OFFSET 0x2
#define LBA_LOW_OFFSET 0x3
#define LBA_MID_OFFSET 0x4
#define LBA_HIGH_OFFSET 0x5
#define DEVICE_PORT_OFFSET 0x6
#define COMMAND_PORT_OFFSET 0x7
#define CONTROL_PORT_OFFSET 0x206

// THIS SHOULD BE DONE USING BIOS INTERRUPTS

int test_ata(unsigned short port, int master) {
	outb(port + DEVICE_PORT_OFFSET, master ? 0xA0 : 0xB0);
	outb(port + CONTROL_PORT_OFFSET, 0);

	outb(port + DEVICE_PORT_OFFSET, 0xA0);
	unsigned char status = inb(port + COMMAND_PORT_OFFSET);
	if (status == 0xff) {
		return 0;
	}

	outb(port + DEVICE_PORT_OFFSET, master ? 0xA0 : 0xB0);
	outb(port + SECTOR_COUNT_OFFSET, 0);
	outb(port + LBA_LOW_OFFSET, 0);
	outb(port + LBA_MID_OFFSET, 0);
	outb(port + LBA_HIGH_OFFSET, 0);
	outb(port + COMMAND_PORT_OFFSET, 0xEC);

	status = inb(port + COMMAND_PORT_OFFSET);
	if (status == 0x00) {
		return 0;
	}

	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = inb(port + COMMAND_PORT_OFFSET);
	}

	if(status & 0x01) {
		return 0;
	}

	for(int i = 0; i < 256; i++) {
		unsigned short dev_data = inw(port + DATA_PORT_OFFSET);
		char* text = (char*) "  \0";
		text[0] = (dev_data >> 8) & 0xFF;
		text[1] = dev_data & 0xFF;
	}

	return 1;
}

void read28_ata(unsigned short port, int master, unsigned int sector, unsigned char* buffer) {
	if(sector & 0xF0000000) {
		return;
	}

	outb(port + DEVICE_PORT_OFFSET, (master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	outb(port + ERROR_PORT_OFFSET, 0);
	outb(port + SECTOR_COUNT_OFFSET, 1);

	outb(port + LBA_LOW_OFFSET, sector & 0x000000FF);
	outb(port + LBA_MID_OFFSET, (sector & 0x0000FF00) >> 8);
	outb(port + LBA_HIGH_OFFSET, (sector & 0x00FF0000) >> 16);
	outb(port + COMMAND_PORT_OFFSET, 0x20);

	unsigned char status = inb(port + COMMAND_PORT_OFFSET);
	while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
		status = inb(port + COMMAND_PORT_OFFSET);
	}

	if(status & 0x01) {
		puts("ATA: Read error\n");
		return;
	}

	for(unsigned short i = 0; i < 512; i += 2) {
		unsigned short wdata = inw(port + DATA_PORT_OFFSET);
		
		buffer[i] = wdata & 0x00FF;
		buffer[i + 1] = (wdata >> 8) & 0x00FF;
	}
}