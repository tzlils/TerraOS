// #include "../include/stdint.h"
#include "../include/memory.h"
#include "../include/utils.h"
#include "../include/string.h"
#include "../include/stdio.h"
#include <stdint.h>

#define MAX_PAGE_ALIGNED_ALLOCS 32
#define PAGE_SIZE ((size_t)4096)
#define PAGE_TABLE_ENTRIES 512
#define KERNEL_PHYS_OFFSET ((size_t)0xffffffffc0000000)

uintptr_t last_alloc = 0;
uintptr_t heap_end = 0;
uintptr_t heap_begin = 0;
uintptr_t pheap_begin = 0;
uintptr_t pheap_end = 0;
uint8_t *pheap_desc = 0;
uint64_t memory_used = 0;

void init_memory(uint64_t kernel_end) {
	last_alloc = kernel_end + 0x1000;
	heap_begin = last_alloc;
	pheap_end = 0x400000;
	pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
	heap_end = pheap_begin;
	printf("Heap begin: %x\nHeap end: %x\nbegin+end: %x\n", heap_begin, heap_end, heap_begin-heap_end);
	memset((char *)heap_begin, 0, heap_end - heap_begin);
	pheap_desc = (uint8_t *)malloc(MAX_PAGE_ALIGNED_ALLOCS);
}


void free(void *mem)
{
	alloc_t *alloc = (mem - sizeof(alloc_t));
	memory_used -= alloc->size + sizeof(alloc_t);
	alloc->status = 0;
}

char* malloc(size_t size)
{
	if(!size) return 0;

	uint8_t *mem = (uint8_t *)heap_begin;
	while((uint32_t)mem < last_alloc)
	{
		alloc_t *a = (alloc_t *)mem;
		if(!a->size)
			goto nalloc;
		if(a->status) {
			mem += a->size;
			mem += sizeof(alloc_t);
			mem += 4;
			continue;
		}
		if(a->size >= size)
		{
			a->status = 1;

			printf("RE:Allocated %d bytes from 0x%x to 0x%x\n", size, mem + sizeof(alloc_t), mem + sizeof(alloc_t) + size);
			memset(mem + sizeof(alloc_t), 0, size);
			memory_used += size + sizeof(alloc_t);
			return (char *)(mem + sizeof(alloc_t));
		}
		mem += a->size;
		mem += sizeof(alloc_t);
		mem += 4;
	}

	nalloc:;
	if(last_alloc+size+sizeof(alloc_t) >= heap_end)
	{
		PANIC("Cannot allocate bytes! Out of memory.\n");
	}
	alloc_t *alloc = (alloc_t *)last_alloc;
	alloc->status = 1;
	alloc->size = size;

	last_alloc += size;
	last_alloc += sizeof(alloc_t);
	last_alloc += 4;
	printf("Allocated %d bytes from 0x%x to 0x%x\n", size, (uint32_t)alloc + sizeof(alloc_t), last_alloc);
	memory_used += size + 4 + sizeof(alloc_t);
	memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);
	return (char *)((uint32_t)alloc + sizeof(alloc_t));
/*
	char* ret = (char*)last_alloc;
	last_alloc += size;
	if(last_alloc >= heap_end)
	{
		set_task(0);
		panic("Cannot allocate %d bytes! Out of memory.\n", size);
	}
	mprint("Allocated %d bytes from 0x%x to 0x%x\n", size, ret, last_alloc);
	return ret;*/
}