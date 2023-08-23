#include "string.h"

int strcmp(char* str1, char* str2) {
	while (*str1 && *str2) {
		if (*str1 != *str2)
			return *str1 - *str2;
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

void* memcpy(void* dest, const void* src, int n) {
	char* d = (char*) dest;
	char* s = (char*) src;
	while(n--) {
		*d++ = *s++;
	}
	return dest;
}

void* memset(void* start, unsigned char value, int num) {
	char* s = (char*) start;
	while(num--) {
		*s++ = value;
	}
	return start;
}