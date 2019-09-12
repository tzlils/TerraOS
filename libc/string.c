#include "../include/string.h"
#include "../include/stdint.h"
 
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++) {
		buf[i] = (unsigned char) value;
    }
	return bufptr;
}

char *strcpy(char *dst, const char *src) {
    char c;
    char *p = dst;

    while ((c = *src++))
    {
        *p++ = c;
    }

    *p = '\0';
    return dst;
}

void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *p = (uint8_t *)src;
    uint8_t *q = (uint8_t *)dst;
    uint8_t *end = p + n;

    while (p != end)
    {
        *q++ = *p++;
    }

    return dst;
}

char * strcat(char *dest, const char *src) {
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}