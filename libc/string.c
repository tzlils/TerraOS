#include "../include/string.h"
#include "../include/stdint.h"
 
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memset (void * ptr, int value, size_t num ) {
	unsigned char* p = ptr;
	while(num--)
		*p++ = (unsigned char)value;
	return ptr;
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

size_t strsplit(char *str, char delim) {
	size_t n = 0;
	uint32_t i = 0;
	while(str[i])
	{
		if(str[i] == delim)
		{
			str[i] = 0;
			n++;
		}
		i++;
	}
	n++;
	return n;
}

size_t strcmp(char* str1, char* str2) {
	size_t res=0;
	while (!(res = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2)
		++str1, ++str2;

	return res;
}


size_t str_backspace(char* str, char c) {
	size_t i = strlen(str);
	i--;
	while(i)
	{
		i--;
		if(str[i] == c)
		{
			str[i+1] = 0;
			return 1;
		}
	}
	return 0;
}

void strreverse(char* str) {
	size_t i;
	char* newstr;
	for (i = strlen(str); i > 0; i--) {
		newstr[i] = str[i];
	}
	str = newstr;
}