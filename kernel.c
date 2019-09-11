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

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif


extern uint32_t kernel_end;
extern uint32_t kernel_base;
extern void _long_mode_init();
extern void _loop_page_tables();

void early_main() {
    terminal_initialize();
    // init_paging();

    // breakpoint();
    // _loop_page_tables();
    // kernel_main();
}

void kernel_main() {
    // set_vmode(1280, 720);
    move_cursor(0, 0);
    printf("Hello world!\n\n");
    
    init_memory(&kernel_end);

    // init_paging();
    _long_mode_init();
    init_idt();
    // init_exceptions();
    // remap_pic();
    // init_pit();
    // init_tasking();

    //Go up to kernel_end
    // init_dev();

    // init_keyboard();
    // printf("\nEverything is initialized\n\n");
    // dev_stats();    

    //pci_probe();
    while(1);
}

void late_init() {
    printf("REACHED LATE INIT!!");
}