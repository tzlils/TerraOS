#include <stdint.h>
#include <stdio.h>

void init_idt();
#define DIVISION_BY_ZERO            0
#define DEBUG_EXCEPTION             1
#define NON_MASKABLE_INTERRUPT      2
#define BREAKPOINT_EXCEPTION        3
#define INTO_DETECTED_OVERFLOW      4
#define OUT_OF_BOUNDS_EXCEPTION     5
#define INVALID_OPCODE_EXCEPTION    6
#define NO_COPROCESSOR_EXCEPTION    7
#define DOUBLE_FAULT                8
#define COPROCESSOR_SEGMENT_OVERRUN 9
#define BAD_TSS                     10
#define SEGMENT_NOT_PRESENT         11
#define STACK_FAULT                 12
#define GENERAL_PROTECTION_FAULT    13
#define PAGE_FAULT                  14
#define UNKNOWN_INTERRUPT_EXCEPTION 15
#define COPROCESSOR_FAULT           16
#define ALIGNMENT_CHECK_EXCEPTION   17
#define MACHINE_CHECK_EXCEPTION     18