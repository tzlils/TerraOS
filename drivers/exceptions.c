#include "../include/exceptions.h";
#include "../include/stdint.h"
#include "../include/stdio.h"

void divide_by_zero(struct system_frame *r) {
    printf("Division by zero!");
}

void debug(struct system_frame *r) {
    printf("DEBUG\n");
}

void NMI(struct system_frame *r) {
    printf("NMI!\N");
}

void exc_breakpoint(struct system_frame *r) {
    printf("Breakpoint!\n");
}

void overflow(struct system_frame *r) {
    printf("Overflow!\n");
}

void bound_range(struct system_frame *r) {
    printf("Bound range exceeded!\n");
}

void invalid_opcode(struct system_frame *r) {
    printf("Invalid opcode!\n");
}

void device_not_available(struct system_frame *r) {
    printf("Device not available!\n");
}

void double_fault(struct system_frame *r) {
    printf("Double fault!\n");
}

void coproccesor_fault(struct system_frame *r) {
    printf("Coprocessor fault!\n");
}

void invalid_tss(struct system_frame *r) {
    printf("Invalid TSS!\n");
}

void segment_not_present(struct system_frame *r) {
    printf("Segment not present!\n");
}

void stacksegment_fault(struct system_frame *r) {
    printf("Stacksegment fault!\n");
}

void general_protection_fault(struct system_frame *r) {
    printf("General protection fault!\n");
}

void page_fault(struct system_frame *r) {
    printf("Page fault!\n");
}

void reserved(struct system_frame *r) {
    printf("Are we in the future?\n");
}