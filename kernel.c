#include "include/tty.h"
#include "include/stdio.h"
// #include "include/vesa.h"
#include "include/gdt.h"
#include "include/pic.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/memory.h"
#include "include/device.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif


extern uint32_t kernel_end;
extern uint32_t kernel_base;

void kernel_main() {

    terminal_initialize();
    //set_vmode(1280, 720);
    printf("Hello world!\n\n");
    
    
    init_memory(&kernel_end);
    init_idt();
    init_gdt();
    remap_pic();
    init_paging();
    init_dev();

    init_keyboard();
    printf("\nEverything is initialized\n\n");
    dev_stats();    
    while(1);
}