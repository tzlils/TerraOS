#ifndef GDT_H
#define GDT_H
#include "../include/stdint.h"
void init_gdt();
int gdt_set_descriptor();
int gdt_add_descriptor(uint8_t id, uint64_t desc);
uint64_t gdt_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);

#endif