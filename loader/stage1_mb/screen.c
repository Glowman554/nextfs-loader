#include "screen.h"

void putc(char c) {
    static int x = 0;
    static int y = 0;
    static char* video = (char*) 0xb8000;

    if ((c == '\n') || (x > 79)) {
        x = 0;
        y++;
    }

    if (c == '\n') {
        return;
    }

    if (y > 25) {
        int i;
        for (i = 0; i < 2 * 25 * 80; i++) {
            video[i] = video[i + 160];
        }

        for (; i < 2 * 25 * 80; i++) {
            video[i] = 0;
        }
        y--;
    }

    video[2 * (y * 80 + x)] = c;
    video[2 * (y * 80 + x) + 1] = 0x0f;

    x++;
}

void puts(const char* s) {
    while (*s) {
        putc(*s++);
    }
}
