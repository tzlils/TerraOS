bits 32
global _start, _long_mode_init, _loop_page_tables, end_of_kernel

%define kernel_phys_offset 0xffffffffc0000000
%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
%define CODE_SEG     0x0008
%define DATA_SEG     0x0010
extern early_main
extern gdt_ptr  
extern gdt_ptr_lowerhalf
extern kernel_main
extern paging_init
extern page_tables

section .data
MBALIGN		equ  1<<0             ; align loaded modules on page boundaries
MEMINFO		equ  1<<1             ; provide memory map
FLAGS		equ  MBALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC		equ  0x1BADB002       ; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot
jmp_up	 	equ (_long_mode_init - kernel_phys_offset)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM


section .bss
stack_bottom:
resb 32768 
stack_top:

section .text
ALIGN 4
IDT:
    .Length       dw 0
    .Base         dd 0    

_start:
	mov esp, stack_top
	call paging_init
	lgdt [gdt_ptr_lowerhalf - kernel_phys_offset]
	jmp 0x08:_long_mode_init - kernel_phys_offset
	; call early_main

	; # Call the global constructors.
	; # call _init

	; # Transfer control to the main kernel.
	; # sti 
	; # sysenter


	; # Hang if kernel_main unexpectedly returns.
	cli
	hlt

_long_mode_init:
	bits 64
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

	mov rax, .higher_half
	jmp rax

.higher_half:
	mov rsp, kernel_phys_offset + 0xeffff0
	lgdt [gdt_ptr]
	call kernel_main

section .kend
end_of_kernel:
