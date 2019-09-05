#ifndef _STRING_H
#define _STRING_H 1
  
#include <stddef.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
// int memcmp(const void*, const void*, size_t);
// void* memmove(void*, const void*, size_t);
// void* memset(void*, int, size_t);
size_t strlen(const char* str);
char *strcpy(char *dst, const char *src);
// void *memcpy(void *dst, const void *src, size_t n);
char * strcat(char *dest, const char *src);
 
#ifdef __cplusplus
}
#endif
 
#endif