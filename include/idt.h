#ifndef IDT_H
#define IDT_H
#include "../include/stdint.h"
#include "../include/stddef.h"

struct system_frame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;  /* pushed by 'pushall' */
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;  /* pushed by 'pushall' */
    uint64_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint64_t rip, cs, eflags, userrsp, ss;   /* pushed by the processor automatically */
};



void init_idt();
int register_interrupt_handler(size_t vec, void (*handler)(void), uint8_t ist, uint8_t type);

#define INT_START asm volatile("pusha");
#define INT_END asm volatile("popa"); \
	asm volatile("iret");
#endif