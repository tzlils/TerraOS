bits 64
global gdt_ptr
section .data

gdt_ptr:
    dw .gdt_end - .gdt_start - 1  ; GDT size
    dq .gdt_start                 ; GDT start

align 16
.gdt_start:
    .null_descriptor:
        dw 0x0000            
        dw 0x0000            
        db 0x00              
        db 00000000b         
        db 00000000b         
        db 0x00              


    .kernel_code_64:
        dw 0x0000            
        dw 0x0000            
        db 0x00              
        db 10011010b         
        db 00100000b         
        db 0x00              

    .kernel_data:
        dw 0x0000            
        dw 0x0000            
        db 0x00              
        db 10010010b         
        db 00000000b         
        db 0x00              
.gdt_end: