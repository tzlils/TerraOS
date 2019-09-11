#include "../include/gdt.h"
#include "../include/stdio.h"

static uint32_t gdt_pointer = 0;
static uint32_t  gdt_size = 0;
static uint32_t gdtr_loc = 0;

static uint32_t highpart = 0;
static uint32_t lowpart = 0;
extern void _reload_segments();
void init_gdt() {
	gdt_pointer = 0x806; // start GDT data at 4MB
	printf("location of GDT: 0x%x\n", gdt_pointer);
	gdtr_loc =    0x800;
	printf("location of GDTR: 0x%x\n", gdtr_loc);
	gdt_add_descriptor(0, 0);
	gdt_add_descriptor(1, 0x00CF9A000000FFFF);
	gdt_add_descriptor(2, 0x00CF92000000FFFF);
	gdt_add_descriptor(3, 0x008FFA000000FFFF);
	gdt_set_descriptor(4, 0x008FF2000000FFFF);
}

int gdt_set_descriptor() {
	/* GDTR
	 * 0-1 = SIZE - 1
	 * 2-5 = OFFSET
	 */
	*(uint16_t*)gdtr_loc = (gdt_size - 1) & 0x0000FFFF;
	gdtr_loc += 2;
	*(uint32_t*)gdtr_loc = gdt_pointer;
	asm volatile("lgdt 0x800");
	printf("GDTR was set. gdtr.size=%d gdtr.offset=0x%x\n", 
		*(uint16_t*)(gdtr_loc-2) + 1, 
		*(uint32_t*)gdtr_loc);
	//_reload_segments();
	printf("Segments reloaded.\n");
	return 0;
}


int gdt_add_descriptor(uint8_t id, uint64_t desc) {
	uint32_t loc = gdt_pointer + sizeof(uint64_t)*id;
	*(uint64_t*)loc = desc;
	printf("Added entry %d = 0x%x << 32 | 0x%x\n", id, (*(uint64_t*)loc) >> 32, *(uint32_t*)loc+4);
	gdt_size += sizeof(desc);
	return 0;
}