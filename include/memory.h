#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../include/stdint.h"
#include <stddef.h>

typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

extern void init_paging();
void init_memory(uint32_t kernel_end);
extern char* malloc(size_t size);
extern void free(void *mem);

extern void* memcpy(const void* dest, const void* src, size_t num );
extern void* memset (void * ptr, int value, size_t num );
extern void* memset16 (void *ptr, uint16_t value, size_t num);

#endif