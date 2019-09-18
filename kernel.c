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
#include "include/memory.h"

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
    

    // initialize_kheap(512);
    init_paging(512, KERNEL_END, KERNEL_VIRTUAL_BASE-KERNEL_END);
    init_idt();
    init_isr();
    init_irq();
    

    init_dev();
    init_keyboard();
    init_pit();

    vfs_init();
    device_t *filesystem = (device_t *)kmalloc(sizeof(device_t));
    filesystem->name = "filesystem";
    filesystem->dev_type = DEVICE_BLOCK;
    filesystem->read = 0;
    filesystem->write = 0;
    filesystem->unique_id = 0x1337;
    construct_ext2_filesystem(filesystem);
    device_add(filesystem);
    vfs_mount_device(filesystem, "/");
    // printf("%s", filesystem->fs->name);
    // list_mount();
    vfs_touch("/test/test.txt");

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