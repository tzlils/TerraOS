#include "../include/memory.h"
#include "../include/stdint.h"
#include "../include/frame.h"
#include "../include/kmalloc_early.h"

struct page_directory *kernel_directory;
struct page_directory *current_directory;
extern uint32_t placement_address;

extern void enable_paging();

uint8_t initialized = 0;

void identity_map(uint32_t address, uint32_t length) {
    uint32_t curr = address & 0xFFFFF000;
    while(curr < address + length) {
        struct page *p = get_page(curr, 1, kernel_directory);
        p->present = 1;
        p->rw = 1;
        p->user = 0;
        p->frame = curr >> 12;
        curr += 0x1000;
    }
}

void init_paging(uint32_t total_frames, uint32_t ident_addr, uint32_t ident_len) {
    init_frame_allocator(total_frames);

    // Make a page directory for the kernel.
    kernel_directory = (struct page_directory *)e_kmalloc_a(sizeof (struct page_directory));
    memset(kernel_directory, 0, sizeof (struct page_directory));
    current_directory = kernel_directory;

    printf("Allocating kernel page tables... ");
    uint32_t i = 0;
    for(i = 0; i < 0xFFFFFFFF;) {
        get_page(i, 1, kernel_directory);
        i += 0x1000 * 1024;
        if(i == 0) {
            break;
        }
    }
    printf("Done\n");

    i = 0;
    while(i < placement_address)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    // Before we do anything else, disable the original kmalloc so we don't
    // start leaking into the address space.
    uint32_t heap_start = disable_early_kmalloc();

    printf("KHEAP_START: %x\n", heap_start);

    // bootstrap the kheap with INITIAL_HEAP_PAGE_COUNT pages.
    for(i = 0; i < INITIAL_HEAP_PAGE_COUNT; i++) {
        alloc_frame(get_page(heap_start + (i * 0x1000), 1, kernel_directory), 0, 0);
    }

    // Before we enable paging, we must register our page fault handler bcz yes

    if(ident_addr) {
        identity_map(ident_addr, ident_len);
    }

    // Now, enable pagingingingignging
    
    initialized = 1;

	enable_paging();
    printf("Initializing kheap... ");

    printf("Done\n");
}

struct page *get_page(uint32_t address, int make, struct page_directory *dir) {
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32_t tmp;
        if(!initialized)
        {
            dir->tables[table_idx] = (struct page_table *)e_kmalloc_ap(sizeof(struct page_table), &tmp);
        }
        else
        {
            // dir->tables[table_idx] = (struct page_table *)kmalloc_ap(sizeof(struct page_table), 1, &tmp);
        }
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return NULL;
    }
}

struct page *get_kernel_page(uint32_t address, int make) {
    return get_page(address, make, kernel_directory);
}

struct page *map_kernel_page(uint32_t address, int make) {
    struct page *p = get_page(address, make, kernel_directory);
    if(!p) return NULL;
    alloc_frame(p, 0, 0);
    return p;
}

void unmap_kernel_page(uint32_t address) {
    struct page *p = get_page(address, 0, kernel_directory);
    if(p) {
        free_frame(p);
    }
}

struct page *map_page(uint32_t address, int make)
{
    struct page *p = get_page(address, make, current_directory);
    if(!p) return NULL;
    alloc_frame(p, 0, 0);
    return p;
}

void unmap_page(uint32_t address, int make) {
    struct page *p = get_page(address, make, current_directory);
    if(p) {
        free_frame(p);
    }
}