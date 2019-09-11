#include "../include/stdint.h"
#include "../include/stdio.h"

static uint32_t* page_directory = 0;
static uint32_t page_dir_loc = 0;
static uint32_t* last_page = 0;

void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys)
{
	uint16_t id = virt >> 22;
	for(int i = 0; i < 1024; i++)
	{
		last_page[i] = phys | 3;
		phys += 4096;
	}
	page_directory[id] = ((uint32_t)last_page) | 3;
	last_page = (uint32_t *)(((uint32_t)last_page) + 4096);
	printf("Mapping 0x%x (%d) to 0x%x\n", virt, id, phys);
}

void paging_enable()
{
	asm volatile("mov %%eax, %%cr3": :"a"(page_dir_loc));	
	asm volatile("mov %cr0, %eax");
	asm volatile("orl $0x80000000, %eax");
	asm volatile("mov %eax, %cr0");
}

void init_paging()
{
	printf("Setting up paging\n");
	page_directory = (uint32_t*)0x400000;
	page_dir_loc = (uint32_t)page_directory;
	last_page = (uint32_t *)0x404000;
	for(int i = 0; i < 1024; i++)
	{
		page_directory[i] = 0 | 2;
	}
	paging_map_virtual_to_phys(0, 0);
	paging_map_virtual_to_phys(0x400000, 0x400000);
	// paging_enable();
	// printf("Paging was successfully enabled!\n");
}

/*
uint32_t PML4[512] __attribute__((aligned(4096)));
void create_PML4() {
	int i;
	for (i = 0; i < 512; i++)
	{
		PML4[i] = ((unsigned int)create_PDPT()) | 3;
	}
	
}

uint32_t create_PDPT() {
	uint32_t PDPT[512] __attribute__((aligned(4096)));
	int i;
	for (i = 0; i < 512; i++)
	{
		PDPT[i] = ((unsigned int)create_page_directory()) | 3;
	}
	return PDPT;
}

uint32_t create_page_directory() {
	uint32_t page_directory[512] __attribute__((aligned(4096)));
	blank_page_directory(page_directory);
	int i;
	for (i = 0; i < 512; i++)
	{
		page_directory[i] = ((unsigned int)create_page_table()) | 3;
	}
	return page_directory;
}

void blank_page_directory(uint32_t* directory) {
	int i;
	for (i = 0; i < 512; i++)
	{
		directory[i] = 0x00000002;
	}
	
}

uint32_t create_page_table() {
	uint32_t page_table[512] __attribute__((aligned(4096)));
	unsigned int i;
	for (i = 0; i < 512; i++)
	{
		page_table[i] = (i * 0x1000) | 3;
	}
	return page_table;
}
*/