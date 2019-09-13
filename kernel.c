#include "include/tty.h"
#include "include/stdio.h"
#include "include/gdt.h"
#include "include/pic.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/memory.h"
#include "include/device.h"
#include "include/exceptions.h"
#include "include/pit.h"
#include "include/utils.h"
#include "include/irq.h"
#include "include/isr.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif


extern uint32_t kernel_end;
extern uint32_t KERNEL_VIRTUAL_BASE;

void kernel_main() {    
    terminal_initialize();
    // set_vmode(1280, 720);
    move_cursor(0, 0);
    printf("Hello world!\n\n");
    
    // init_memory(&kernel_end);

    // init_paging();
    init_idt();
    init_isr();
    init_irq();

    // init_dev();
    init_keyboard();
    init_pit();
    while(1) asm ("hlt");
    // init_tasking();

    //Go up to kernel_end

    // init_keyboard();
    // printf("\nEverything is initialized\n\n");
    // dev_stats();    

    //pci_probe();
}

void late_main() {
    printf("REACHED LATE INIT!!");
}
