#ifndef KHEAP_H
#define KHEAP_H

#include "../include/stdint.h"
#define INITIAL_HEAP_PAGE_COUNT 4

void initialize_kheap(uint32_t start_addr);
void *kmalloc(uint32_t size);
void *krealloc(void *p, uint32_t size);
void *kmalloc_ap(uint32_t size, uint8_t page_align, uint32_t *phys);
void kfree(void *p);
void malloc_stats();

#endif