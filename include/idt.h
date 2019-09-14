#ifndef IDT_H
#define IDT_H
#include "../include/stdint.h"
#include "../include/stddef.h"
#include "../include/isr.h"

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

extern void init_idt();
extern void __idt_default_handler();
extern int register_interrupt_handler(size_t vec, void (*handler)(struct system_frame *), uint8_t ist, uint8_t type);

#endif