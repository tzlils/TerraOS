bits 64
global gdt_ptr
section .data

gdt_ptr:
    dw .gdt_end - .gdt_start - 1  ; GDT size
    dq .gdt_start                 ; GDT start

align 16
.gdt_start:
    .null_descriptor:
        dq 0x0000000000000000       


    .kernel_code_64:
        dq 0x00209A0000000000  

    .kernel_data:
        dq 0x0000920000000000           
.gdt_end: