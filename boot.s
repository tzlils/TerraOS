bits 32
global _start, _long_mode_init, _loop_page_tables, end_of_kernel

%define KERNEL_VIRTUAL_BASE 0xFFFFFFFF80000000
%define kernel_phys_offset 0xffffffffc0000000
%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
%define CODE_SEG     0x0008
%define DATA_SEG     0x0010
extern early_main
extern gdt_ptr  
extern kernel_main

section .data
MBALIGN		equ  1<<0             ; align loaded modules on page boundaries
MEMINFO		equ  1<<1             ; provide memory map
FLAGS		equ  MBALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC		equ  0x1BADB002       ; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot
jmp_up	 	equ (_long_mode_init - KERNEL_VIRTUAL_BASE)

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
	call early_main

	; # Call the global constructors.
	; # call _init

	; # Transfer control to the main kernel.
	; # sti 
	; # sysenter


	; # Hang if kernel_main unexpectedly returns.
	cli
	hlt
	
_loop_page_tables:
	mov [0x400000], eax
	add eax, 0x1000
	add di, 8
	cmp eax, 0x200000
	jb _loop_page_tables

	pop di
	mov al, 0xFF
	out 0xA1, al
	out 0x21, al
	nop
	nop

	lidt [IDT]

	mov eax, 10100000b
	mov cr4, eax

	mov edx, edi
	mov cr3, edx

	mov ecx, 0xC0000080
	rdmsr
	or eax, 0x00000100
	wrmsr

	xchg bx, bx
	lgdt [gdt_ptr]

	mov ebx, cr0
	or ebx, 0x80000001
	mov cr0, ebx
	
	jmp CODE_SEG:jmp_up

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
	jmp kernel_main

section .kend
end_of_kernel:
