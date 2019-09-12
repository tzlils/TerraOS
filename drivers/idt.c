#include "../include/idt.h"
#include "../include/memory.h"
#include "../include/tasking.h"
#include "../include/stdio.h"
#include "../include/utils.h"


extern void _set_idtr();
extern void __idt_default_handler();
void idt_dummy_handler();
static uint32_t idt_location = 0;
static uint32_t idtr_location = 0;
static uint16_t idt_size = 0x800;

static uint8_t test_success = 0;
static uint32_t test_timeout = 0x1000;
static uint8_t __idt_setup = 0;

void load_idt(void)
{
    struct
    {
        uint16_t limit;
        void *pointer;
    }
    __attribute__((packed)) idtp =
    {
        .limit = *(uint16_t*)idtr_location * 8 - 1,
        .pointer = 0x2000,
    };
    asm volatile("lidt %0" : : "m" (idtp));
}

void init_idt() {
	idt_location = 0x2000;
	printf("Location: 0x%x\n", idt_location);
	idtr_location = 0x10f0;
	printf("IDTR location: 0x%x\n", idtr_location);
	__idt_setup = 1;
	for(uint8_t i = 0; i < 255; i++)
	{
		idt_register_interrupt(i, (uint32_t)&__idt_default_handler);
	}
	idt_register_interrupt(0x2f, (uint32_t)&idt_dummy_handler);
	idt_register_interrupt(0x2e, (uint32_t)&schedule);
	printf("Registered all interrupts to default handler.\n");
	/* create IDTR now */
	*(uint16_t*)idtr_location = idt_size - 1;
	*(uint32_t*)(idtr_location + 2) = idt_location;
	printf("IDTR.size = 0x%x IDTR.offset = 0x%x\n", *(uint16_t*)idtr_location, *(uint32_t*)(idtr_location + 2));
	load_idt();
	printf("IDTR set, testing link.\n");

	
	asm volatile("sti");
	asm volatile("int $0x2f");
	while(test_timeout-- != 0)
	{
		if(test_success != 0)
		{
			printf("Test succeeded, disabling INT#0x2F\n");
			idt_register_interrupt(0x2F, (uint32_t)&__idt_default_handler);
			break;
		}
	}
	if(!test_success)
		PANIC("IDT link is offline (timeout).");
	return;
}

void idt_register_interrupt(uint8_t i, uint32_t callback)
{
	if(!__idt_setup) PANIC("Invalid IDT!");
	*(uint16_t*)(idt_location + 8*i + 0) = (uint16_t)(callback & 0x0000ffff);
	*(uint16_t*)(idt_location + 8*i + 2) = (uint16_t)0x8;
	*(uint8_t*) (idt_location + 8*i + 4) = 0x00;
	*(uint8_t*) (idt_location + 8*i + 5) = 0x8e;//0 | IDT_32BIT_INTERRUPT_GATE | IDT_PRESENT;
	*(uint16_t*)(idt_location + 8*i + 6) = (uint16_t)((callback & 0xffff0000) >> 16);
	if(test_success) printf("Registered INT#%d\n", i);
	return;
}

void idt_dummy_handler() {
	INT_START;
	printf("Dummy handler\n");
    test_success = 1;
	INT_END;
}