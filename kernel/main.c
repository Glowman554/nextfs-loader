#include "com1.h"

void print(const char* s) {
    while (*s) {
        com1_transmit(*s++);
    }
}

void receive(char* out) {
    while (1) {
        *out = com1_receive();
        if (*out != ' ' && *out != '\n' && *out != '\r') {
            out++;
        } else {
            *out = 0;
            return;
        }
    };
}

unsigned int parse_hex(const char* in) {
    unsigned int result = 0;

    while (*in) {
        char c = *in++;

        if (c >= '0' && c <= '9') {
            result  = result * 16 + (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            result  = result * 16 + (c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            result  = result * 16 + (c - 'A' + 10);
        } else {
            return result;
        }
    }
    
    return result;
}

void print_hex(unsigned int num) {
    if (num < 16) {
        com1_transmit("0123456789ABCDEF"[num]);
    } else {
        print_hex(num / 16);
        com1_transmit("0123456789ABCDEF"[num % 16]);
    }
}


void _main() {
    com1_init();

    print("TestOS v0.1 Copyright (C) 2023 Glowman554\n");

    char buf[32] = { 0 };

    while (1) {
        print("\n@");

        char cmd = com1_receive();
        switch (cmd) {
            case 'G': // GO
                {
                    receive(buf);
                    asm volatile("call %0" :: "r"(parse_hex(buf)));
                }
                break;
            case 'M': // MEMORY
                {
                    receive(buf);

                    unsigned int start = parse_hex(buf);
                    while (1) {
                        print_hex(*((char*) start) & 0xff);
                        receive(buf);
                        if (*buf == '\\') {
                            break;
                        }
                        if (*buf != 0) {
                            *((char*) start) = parse_hex(buf);
                        }

                        start++;
                    }
                }
                break;
            default:
                print("\nERROR");
                break;
        }
    }
}

// MB8000 48 0f 65 0f 6C 0f 6C 0f 6F 0f 20 0f 57 0f 6F 0f 72 0f 6C 0f 64 0f 21 0f
// M8000 c6 05 00 80 0b 00 48 c6 05 02 80 0b 00 65 c6 05 04 80 0b 00 6c c6 05 06 80 0b 00 6c c6 05 08 80 0b 00 6f c3 \ G8000
// M8000 b9 d2 7 0 0 c6 81 0 80 b 0 0 e2 f7 c3 \ G8000
