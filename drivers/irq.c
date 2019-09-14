#include "../include/irq.h"
#include "../include/pic.h"

void irq_default(struct system_frame *r);
void (*irq_routines[16])(struct system_frame *) = { NULL };

void irq_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void init_irq(void)
{
    irq_remap();
    register_interrupt_handler(IRQ0, irq0, 0, 0x8e);
    register_interrupt_handler(IRQ1, irq1, 0, 0x8e);
    register_interrupt_handler(IRQ2, irq2, 0, 0x8e);
    register_interrupt_handler(IRQ3, irq3, 0, 0x8e);
    register_interrupt_handler(IRQ4, irq4, 0, 0x8e);
    register_interrupt_handler(IRQ5, irq5, 0, 0x8e);
    register_interrupt_handler(IRQ6, irq6, 0, 0x8e);
    register_interrupt_handler(IRQ7, irq7, 0, 0x8e);
    register_interrupt_handler(IRQ8, irq8, 0, 0x8e);
    register_interrupt_handler(IRQ9, irq9, 0, 0x8e);
    register_interrupt_handler(IRQ10, irq10, 0, 0x8e);
    register_interrupt_handler(IRQ11, irq11, 0, 0x8e);
    register_interrupt_handler(IRQ12, irq12, 0, 0x8e);
    register_interrupt_handler(IRQ13, irq13, 0, 0x8e);
    register_interrupt_handler(IRQ14, irq14, 0, 0x8e);
    register_interrupt_handler(IRQ15, irq15, 0, 0x8e);
    asm ("sti");
}

void irq_default(struct system_frame *r) {
    printf("Unprocessed IRQ\n");
}

void irq_handler(struct system_frame *r) {
//    printf("GOT IRQ 0x%x\n", r->int_no - 32);
    /* This is a blank function pointer */
    void (*handler)(struct system_frame *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler) {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40) {
        outb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(0x20, 0x20);
}

void send_eoi(uint8_t irq) {
    pic_send_eoi(irq);
}

void register_irq_callback (size_t vec, void (*callback)(struct system_frame *)) {
    irq_routines[vec - 32] = callback;
}