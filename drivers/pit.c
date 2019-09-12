#include "../include/pit.h"
#include "../include/serial.h"
#include "../include/tasking.h"
#include "../include/stdio.h"


static uint8_t task = 0;
static uint8_t task_was_on = 0;


void set_task(uint8_t i) {
	if(!task_was_on) return;
	task = i;
}

void enable_task() {
	task_was_on = 1;
	task = 1;
}

void pit_irq() {
	if(!task) {
		// asm volatile("add $0x1c, %esp");
		// asm volatile("pusha");
		// send_eoi(0);
		// asm volatile("popa");
		// asm volatile("iret");
	} else {
		//asm volatile("add $0x1c, %esp");
		schedule();
	}
}

static inline void __pit_send_cmd(uint8_t cmd)
{
	outb(PIT_REG_COMMAND, cmd);
}

static inline void __pit_send_data(uint16_t data, uint8_t counter) {
	uint8_t	port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 :
		((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);

	outb (port, (uint8_t)data);
}

static void pit_start_counter (uint32_t freq, uint8_t counter, uint8_t mode) {

	if (freq==0)
		return;
	printf("Starting counter %d with frequency %dHz\n", counter/0x40, freq);
	uint16_t divisor = (uint16_t)( 1193181 / (uint16_t)freq);

	// send operational command words
	uint8_t ocw = 0;
	ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
	ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
	__pit_send_cmd (ocw);

	// set frequency rate
	__pit_send_data (divisor & 0xff, 0);
	__pit_send_data ((divisor >> 8) & 0xff, 0);
}

void init_pit() {
	set_int(32, (uint32_t)pit_irq);
	pit_start_counter (200,PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);
	printf("[OS] PIT Ready\n");
}