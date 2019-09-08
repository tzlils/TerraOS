#include "../include/exceptions.h";
#include "../include/idt.h"
#include "../include/stdint.h"
#include "../include/stdio.h"

void divide_by_zero() {
    printf("Division by zero!");
}

void debug() {
    printf("DEBUG\n");
}

void NMI() {
    printf("NMI!\N");
}

void breakpoint() {
    printf("Breakpoint!\n");
}

void overflow() {
    printf("Overflow!\n");
}

void bound_range() {
    printf("Bound range exceeded!\n");
}

void invalid_opcode() {
    printf("Invalid opcode!\n");
}

void device_not_available() {
    printf("Device not available!\n");
}

void double_fault() {
    printf("Double fault!\n");
}

void coproccesor_fault() {
    printf("Coprocessor fault!\n");
}

void invalid_tss() {
    printf("Invalid TSS!\n");
}

void segment_not_present() {
    printf("Segment not present!\n");
}

void stacksegment_fault() {
    printf("Stacksegment fault!\n");
}

void general_protection_fault() {
    printf("General protection fault!\n");
}

void page_fault() {
    printf("Page fault!\n");
}

void reserved() {
    printf("Are we in the future?\n");
}

void init_exceptions() {
    idt_register_interrupt(0, (uint32_t)divide_by_zero);
    idt_register_interrupt(1, (uint32_t)debug);
    idt_register_interrupt(2, (uint32_t)NMI);
    idt_register_interrupt(3, (uint32_t)breakpoint);
    idt_register_interrupt(4, (uint32_t)overflow);
    idt_register_interrupt(5, (uint32_t)bound_range);
    idt_register_interrupt(6, (uint32_t)invalid_opcode);
    idt_register_interrupt(7, (uint32_t)device_not_available);
    idt_register_interrupt(8, (uint32_t)double_fault);
    idt_register_interrupt(9, (uint32_t)coproccesor_fault);
    idt_register_interrupt(10, (uint32_t)invalid_tss);
    idt_register_interrupt(11, (uint32_t)segment_not_present);
    idt_register_interrupt(12, (uint32_t)stacksegment_fault);
    idt_register_interrupt(13, (uint32_t)general_protection_fault);
    idt_register_interrupt(14, (uint32_t)page_fault);
    idt_register_interrupt(15, (uint32_t)reserved);
    printf("[OS] Exceptions ready\n");
}