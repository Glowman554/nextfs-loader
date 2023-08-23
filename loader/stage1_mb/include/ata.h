#pragma once

int test_ata(unsigned short port, int master);
void read28_ata(unsigned short port, int master, unsigned int sector, unsigned char* buffer);