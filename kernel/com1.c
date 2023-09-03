#include "com1.h"
#include "io.h"

#define PORT 0x3f8   /* COM1 */

void com1_init() {
    outb(PORT + 1, 0x00);    // Disable all interrupts
	outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00);    //                  (hi byte)
	outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int com1_received() {
	return inb(PORT + 5) & 1;
}

char com1_receive() {
	while(com1_received() == 0);
	char c = inb(PORT);
	com1_transmit(c);
	return c;
}

int com1_is_transmit_empty() {
	return inb(PORT + 5) & 0x20;
}

void com1_transmit(char c) {
	while(com1_is_transmit_empty() == 0);
	outb(PORT, c);
}
