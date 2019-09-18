#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../include/stdint.h"
#include <stddef.h>

#define INITIAL_HEAP_PAGE_COUNT 4

typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

struct page {
    uint32_t present    : 1;   // Page present in memory
    uint32_t rw         : 1;   // Read-only if clear, readwrite if set
    uint32_t user       : 1;   // Supervisor level only if clear
    uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
    uint32_t dirty      : 1;   // Has the page been written to since last refresh?
    uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
    uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
};

struct page_table {
    struct page pages[1024];
};

struct page_directory {
    struct page_table *tables[1024];
    uint32_t tablesPhysical[1024];
    uint32_t physicalAddr;
};

struct	boundary_tag
{
	unsigned int magic;			//< It's a kind of ...
	unsigned int size; 			//< Requested size.
	unsigned int real_size;		//< Actual size.
	int index;					//< Location in the page table.

	struct boundary_tag *split_left;	//< Linked-list info for broken pages.	
	struct boundary_tag *split_right;	//< The same.
	
	struct boundary_tag *next;	//< Linked list info.
	struct boundary_tag *prev;	//< Linked list info.
};

void init_paging(uint64_t total_frames, uint64_t ident_addr, uint64_t ident_len);
extern void paging_enable();
struct page *get_page(uint32_t address, int make, struct page_directory *dir);
struct free_header *find_block(uint32_t size, uint8_t align);
struct page *map_page(uint32_t address, int make);
void unmap_page(uint32_t address, int make);

struct page *get_kernel_page(uint32_t address, int make);
struct page *map_kernel_page(uint32_t address, int make);
void unmap_kernel_page(uint32_t address);

// extern void* memcpy(const void* dest, const void* src, size_t num );
// extern void* memset (void * ptr, int value, size_t num );
extern void* memset16 (void *ptr, uint16_t value, size_t num);

void     *malloc(size_t);				//< The standard function.
void     *realloc(void *, size_t);		//< The standard function.
void     *calloc(size_t, size_t);		//< The standard function.
void      free(void *);					//< The standard function.
#endif