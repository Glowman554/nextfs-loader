void _start(void) {
	const char hw[] = "Hello 32 bit kernel World! :)";
	char* video = (char*) 0xb8000;

	for (int i = 0; i < 25 * 80; i++) {
		video[2*i] = ' ';
		video[2*i+1] = 0x07;
	}


	for (int i = 0; hw[i] != '\0'; i++) {

		video[i * 2] = hw[i];

		video[i * 2 + 1] = 0x07;
	}

	while(1) {
		__asm__ __volatile__("hlt;");
	}
}