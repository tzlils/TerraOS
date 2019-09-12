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

void exc_breakpoint() {
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
    register_interrupt_handler(0, divide_by_zero, 0, 0x8e);
    register_interrupt_handler(1, debug, 0, 0x8e);
    register_interrupt_handler(2, NMI, 0, 0x8e);
    register_interrupt_handler(3, exc_breakpoint, 0, 0x8e);
    register_interrupt_handler(4, overflow, 0, 0x8e);
    register_interrupt_handler(5, bound_range, 0, 0x8e);
    register_interrupt_handler(6, invalid_opcode, 0, 0x8e);
    register_interrupt_handler(7, device_not_available, 0, 0x8e);
    register_interrupt_handler(8, double_fault, 0, 0x8e);
    register_interrupt_handler(9, coproccesor_fault, 0, 0x8e);
    register_interrupt_handler(10, invalid_tss, 0, 0x8e);
    register_interrupt_handler(11, segment_not_present, 0, 0x8e);
    register_interrupt_handler(12, stacksegment_fault, 0, 0x8e);
    register_interrupt_handler(13, general_protection_fault, 0, 0x8e);
    register_interrupt_handler(14, page_fault, 0, 0x8e);
    register_interrupt_handler(15, reserved, 0, 0x8e);
    printf("[OS] Exceptions ready\n");
}