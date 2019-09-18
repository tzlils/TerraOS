#include "include/tty.h"
#include "include/stdio.h"
#include "include/gdt.h"
#include "include/pic.h"
#include "include/idt.h"
#include "include/keyboard.h"
#include "include/kheap.h"
#include "include/device.h"
#include "include/exceptions.h"
#include "include/pit.h"
#include "include/utils.h"
#include "include/irq.h"
#include "include/isr.h" 
#include "include/vfs.h"
#include "include/ext2.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif


extern uint64_t KERNEL_END;
extern uint64_t KERNEL_VIRTUAL_BASE;
extern void init_shell();

void kernel_main() {    
    terminal_initialize();
    // set_vmode(1280, 720);
    move_cursor(0, 0);

    //Thanks slendi
    printf("Kernel has joined the game\n\n");
    

    initialize_kheap(256);
    init_idt();
    init_isr();
    init_irq();
    

    init_dev();
    init_keyboard();
    init_pit();

    vfs_init();
    device_t *filesystem = (device_t *)kmalloc(sizeof(device_t));
    filesystem->name = "Filesystem";
    device_add(filesystem);
    construct_ext2_filesystem(filesystem);
    vfs_mount_device(filesystem, "/test");
    filesystem->fs->touch("test", filesystem, filesystem->fs->metadata);


    printf("STARING SHELL\n");
    init_shell();

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