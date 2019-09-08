#ifndef IDT_H
#define IDT_H
#include "../include/stdint.h"

typedef struct {
	uint16_t offset_0_15;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_16_31;
} __attribute__((packed)) idt_descriptor;

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

void init_idt();
void idt_register_interrupt(uint8_t i, uint32_t addr);
void idt_dummy_handler();

#define INT_START asm volatile("pusha");
#define INT_END asm volatile("popa"); \
	asm volatile("iret");
#endif