#include <stdint.h>

void init_gdt();
void gdt_set_gate(int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);