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

MBALIGN		equ  1<<0             ; align loaded modules on page boundaries
MEMINFO		equ  1<<1             ; provide memory map
FLAGS		equ  MBALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC		equ  0x1BADB002       ; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot
jmp_up	 	equ (_long_mode_init - kernel_phys_offset)

section .multiboot.data
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .multiboot.text
ALIGN 4
IDT:
    .Length       dw 0
    .Base         dd 0    

_start:
	mov esp, stack_top - kernel_phys_offset
	lgdt [gdt_ptr_lowerhalf]
	call paging_init
	jmp 0x08:_long_mode_init
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

	add rsp, kernel_phys_offset
	lgdt [gdt_ptr]
	and rsp, -16        ; Align stack on 128 byte boundary

	mov rax, cr0
	and ax, 0xFFFB          ;clear coprocessor emulation CR0.EM
	or ax, 0x2                      ;set coprocessor monitoring  CR0.MP
	mov cr0, rax
	mov rax, cr4
	or ax, 3 << 9           ;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
	mov cr4, rax

	mov rax, kernel_main
	call rax
	cli
	hlt

section .bss
stack_bottom:
	resb 32768
stack_top:

section .kend
end_of_kernel:
