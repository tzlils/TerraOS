#include "../include/idt.h"
#include "../include/memory.h"
#include "../include/tasking.h"
#include "../include/stdio.h"
#include "../include/utils.h"


struct idt_entry {
	uint16_t offset_1;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_attr;
	uint16_t offset_2;
	uint32_t offset_3;
	uint32_t zero;
};

struct idt_ptr {
	uint16_t limit;
	uint64_t offset;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;
extern void __idt_default_handler();

void init_idt() {
	for(uint8_t i = 0; i < 255; i++)
	{
		register_interrupt_handler(i, __idt_default_handler, 0, 0x8e);
	}

    struct idt_ptr idt_ptr = {
        sizeof(idt) - 1,
        (uint64_t)idt
    };

	asm volatile (
        "lidt %0"
        :
        : "m" (idt_ptr)
    );
}


int register_interrupt_handler(size_t vec, void (*handler)(void), uint8_t ist, uint8_t type) {
    uint64_t p = (uint64_t)handler;

    idt[vec].offset_1 = (uint16_t)p;
    idt[vec].selector = 0x08;
    idt[vec].ist = ist;
    idt[vec].type_attr = type;
    idt[vec].offset_2 = (uint16_t)(p >> 16);
    idt[vec].offset_3 = (uint32_t)(p >> 32);
    idt[vec].zero = 0;

    return 0;
}