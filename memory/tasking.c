#include "../include/tasking.h"
#include "../include/memory.h"
#include "../include/pit.h"
#include "../include/utils.h"


uint32_t lastpid = 0;
process* c = 0;
uint8_t __enabled = 0;
extern void late_main();

process* create_process(char* name, uint32_t addr) {
    process* p = (process *)malloc(sizeof(process));
    memset(p, 0, sizeof(process));
    p->name = name;
    p->pid = ++lastpid;
    p->eip = addr;
    p->state = STATE_ALIVE;
    p->esp = (uint32_t)malloc(4096);
    // asm volatile("mov %%cr3, %%eax":"=a"(p->cr3));
    uint32_t* stack = (uint32_t *)(p->esp + 4096);
    p->stacktop = p->esp;
	*--stack = 0x00000202; // eflags
	*--stack = 0x8; // cs
	*--stack = (uint32_t)addr; // eip
	*--stack = 0; // eax
	*--stack = 0; // ebx
	*--stack = 0; // ecx;
	*--stack = 0; //edx
	*--stack = 0; //esi
	*--stack = 0; //edi
	*--stack = p->esp + 4096; //ebp
	*--stack = 0x10; // ds
	*--stack = 0x10; // fs
	*--stack = 0x10; // es
	*--stack = 0x10; // gs
	p->esp = (uint32_t)stack;
    return p;
}

void add_process_prime(process* p) {
    p->next = c->next;
	p->next->prev = p;
	p->prev = c;
	c->next = p;
}

void add_process(process* p) {

}

void schedule() {
	// asm volatile("add $0xc, %esp");
	// asm volatile("push eax");
	// asm volatile("push ebx");
	// asm volatile("push ecx");
	// asm volatile("push edx");
	// asm volatile("push esi");
	// asm volatile("push edi");
	// asm volatile("push ebp");
	// asm volatile("push ds");
	// asm volatile("push es");
	// asm volatile("push fs");
	// asm volatile("push gs");
	// asm volatile("mov %%esp, %%eax":"=a"(c->esp));
	// c = c->next;
	// // asm volatile("mov %%eax, %%cr3": :"a"(c->cr3));
	// asm volatile("mov %%eax, %%esp": :"a"(c->esp));
	// asm volatile("pop gs");
	// asm volatile("pop fs");
	// asm volatile("pop es");
	// asm volatile("pop ds");
	// asm volatile("pop ebp");
	// asm volatile("pop edi");
	// asm volatile("pop esi");
	// asm volatile("out %%al, %%dx": :"d"(0x20), "a"(0x20)); // send EoI to master PIC
	// asm volatile("pop edx");
	// asm volatile("pop ecx");
	// asm volatile("pop ebx");
	// asm volatile("pop eax");
	// asm volatile("iret");
}

void schedule_noirq()
{
	if(!__enabled) return;
	asm volatile("int $0x2e");
	return;
}

void __exec()
{
	// asm volatile("mov %%eax, %%esp": :"a"(c->esp));
	// asm volatile("pop %gs");
	// asm volatile("pop %fs");
	// asm volatile("pop %es");
	// asm volatile("pop %ds");
	// asm volatile("pop %ebp");
	// asm volatile("pop %edi");
	// asm volatile("pop %esi");
	// asm volatile("pop %edx");
	// asm volatile("pop %ecx");
	// asm volatile("pop %ebx");
	// asm volatile("pop %eax");
	// asm volatile("iret");
}


void idle_thread() {
	enable_task();
	__enabled = 1;
	late_main();
}

void hunter_thread()
{
	reset:;
	process *orig = c;
	process *p = orig;
	while(1)
	{
		p = p->next;
		if(p == c) { continue;}
		if(p->state == STATE_ZOMBIE)
		{
			set_task(0);
			p->prev->next = p->next;
			p->next->prev = p->prev;
			free((void *)p->stacktop);
			free(p);
			set_task(1);
			// printf("Hunter killed %s (%d). One less zombie.\n", p->name, p->pid);
		}
		if(p == orig) goto reset;
		schedule_noirq();
	}
}

void task_thread() {
    printf("Task thread is working!");
}


void init_tasking() {
	printf("Creating idle process\n");
	c = create_process("kidle", (uint32_t)idle_thread);
	c->next = c;
	c->prev = c;
	add_process_prime(create_process("task", (uint32_t)task_thread));
	add_process_prime(create_process("Hunter", (uint32_t)hunter_thread));
	__exec();
	PANIC("Failed to start tasking!");
}