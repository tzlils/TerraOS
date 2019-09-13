#include "../include/serial.h"
#include "../include/isr.h"
#include "../include/exceptions.h"

void (*isr_routines[32])(struct system_frame *r) =
{
	divide_by_zero,
	debug,
	NMI,
	exc_breakpoint,
	overflow,
	bound_range,
	invalid_opcode,
	device_not_available,
	double_fault,
	coproccesor_fault,
	invalid_tss,
	segment_not_present,
	stacksegment_fault,
	general_protection_fault,
	page_fault,
};

void init_isr(void)
{
	register_interrupt_handler(ISR0, isr0, 0, 0x8e);
	register_interrupt_handler(ISR1, isr1, 0, 0x8e);
	register_interrupt_handler(ISR2, isr2, 0, 0x8e);
	register_interrupt_handler(ISR3, isr3, 0, 0x8e);
	register_interrupt_handler(ISR4, isr4, 0, 0x8e);
	register_interrupt_handler(ISR5, isr5, 0, 0x8e);
	register_interrupt_handler(ISR6, isr6, 0, 0x8e);
	register_interrupt_handler(ISR7, isr7, 0, 0x8e);
	register_interrupt_handler(ISR8, isr8, 0, 0x8e);
	register_interrupt_handler(ISR9, isr9, 0, 0x8e);
	register_interrupt_handler(ISR10, isr10, 0, 0x8e);
	register_interrupt_handler(ISR11, isr11, 0, 0x8e);
	register_interrupt_handler(ISR12, isr12, 0, 0x8e);
	register_interrupt_handler(ISR13, isr13, 0, 0x8e);
	register_interrupt_handler(ISR14, isr14, 0, 0x8e);
	register_interrupt_handler(ISR15, isr15, 0, 0x8e);
	register_interrupt_handler(ISR16, isr16, 0, 0x8e);
	register_interrupt_handler(ISR17, isr17, 0, 0x8e);
	register_interrupt_handler(ISR18, isr18, 0, 0x8e);
	register_interrupt_handler(ISR19, isr19, 0, 0x8e);
	register_interrupt_handler(ISR20, isr20, 0, 0x8e);
	register_interrupt_handler(ISR21, isr21, 0, 0x8e);
	register_interrupt_handler(ISR22, isr22, 0, 0x8e);
	register_interrupt_handler(ISR23, isr23, 0, 0x8e);
	register_interrupt_handler(ISR24, isr24, 0, 0x8e);
	register_interrupt_handler(ISR25, isr25, 0, 0x8e);
	register_interrupt_handler(ISR26, isr26, 0, 0x8e);
	register_interrupt_handler(ISR27, isr27, 0, 0x8e);
	register_interrupt_handler(ISR28, isr28, 0, 0x8e);
	register_interrupt_handler(ISR29, isr29, 0, 0x8e);
	register_interrupt_handler(ISR30, isr30, 0, 0x8e);
	register_interrupt_handler(ISR31, isr31, 0, 0x8e);
    printf("[OS] Exceptions ready\n");
}

void isr_handler(struct system_frame *r) {
    printf("GOT ISR 0x%x\n", r->int_no);
    /* This is a blank function pointer */
    void (*handler)(struct system_frame *r);

    /* Find out if we have a custom handler to run for this
    *  ISR, and then finally, run it */
    handler = isr_routines[r->int_no];
    if (handler) {
        handler(r);
    }
}
