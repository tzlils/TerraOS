#ifndef TASKING_H
#define TASKING_h
#include "../include/stdint.h"

#define STATE_ALIVE 0
#define STATE_ZOMBIE 1
#define STATE_DEAD 2

typedef struct process {
    struct process* prev;
    char* name;
    uint32_t pid;
    uint32_t esp;
    uint32_t stacktop;
    uint32_t eip;
    uint32_t cr3;
    uint32_t state;
    struct process* next;
} process;

void schedule();
void init_tasking();
int addProcess(process* p);
process* createProcess(char* name, uint32_t addr);

#endif