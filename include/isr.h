#ifndef ISR_H
#define ISR_H

#include "../include/stdint.h"
#include <stddef.h>

#define ISR0 0
#define ISR1 1
#define ISR2 2
#define ISR3 3
#define ISR4 4
#define ISR5 5
#define ISR6 6
#define ISR7 7
#define ISR8 8
#define ISR9 9
#define ISR10 10
#define ISR11 11
#define ISR12 12
#define ISR13 13
#define ISR14 14
#define ISR15 15

#define ISR16 16
#define ISR17 17
#define ISR18 18
#define ISR19 19
#define ISR20 20
#define ISR21 21
#define ISR22 22
#define ISR23 23
#define ISR24 24
#define ISR25 25
#define ISR26 26
#define ISR27 27
#define ISR28 28
#define ISR29 29
#define ISR30 30
#define ISR31 31

struct system_frame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;  /* pushed by 'pushall' */
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;  /* pushed by 'pushall' */
    uint64_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint64_t rip, cs, eflags, userrsp, ss;   /* pushed by the processor automatically */
};

int register_interrupt_handler(size_t vec, void (*handler)(struct system_frame *), uint8_t ist, uint8_t type);

extern void isr0(struct system_frame *);
extern void isr1(struct system_frame *);
extern void isr2(struct system_frame *);
extern void isr3(struct system_frame *);
extern void isr4(struct system_frame *);
extern void isr5(struct system_frame *);
extern void isr6(struct system_frame *);
extern void isr7(struct system_frame *);
extern void isr8(struct system_frame *);
extern void isr9(struct system_frame *);
extern void isr10(struct system_frame *);
extern void isr11(struct system_frame *);
extern void isr12(struct system_frame *);
extern void isr13(struct system_frame *);
extern void isr14(struct system_frame *);
extern void isr15(struct system_frame *);
extern void isr16(struct system_frame *);
extern void isr17(struct system_frame *);
extern void isr18(struct system_frame *);
extern void isr19(struct system_frame *);
extern void isr20(struct system_frame *);
extern void isr21(struct system_frame *);
extern void isr22(struct system_frame *);
extern void isr23(struct system_frame *);
extern void isr24(struct system_frame *);
extern void isr25(struct system_frame *);
extern void isr26(struct system_frame *);
extern void isr27(struct system_frame *);
extern void isr28(struct system_frame *);
extern void isr29(struct system_frame *);
extern void isr30(struct system_frame *);
extern void isr31(struct system_frame *);

extern void divide_by_zero(struct system_frame *);
extern void debug(struct system_frame *);
extern void NMI(struct system_frame *);
extern void exc_breakpoint(struct system_frame *);
extern void overflow(struct system_frame *);
extern void bound_range(struct system_frame *);
extern void invalid_opcode(struct system_frame *);
extern void device_not_available(struct system_frame *);
extern void double_fault(struct system_frame *);
extern void coproccesor_fault(struct system_frame *);
extern void invalid_tss(struct system_frame *);
extern void segment_not_present(struct system_frame *);
extern void stacksegment_fault(struct system_frame *);
extern void general_protection_fault(struct system_frame *);
extern void page_fault(struct system_frame *);
extern void reserved(struct system_frame *);
extern void init_isr(void);

#endif
