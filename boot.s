bits 32
global _start, _long_mode_init, end_of_kernel

section .data
MBALIGN		equ  1<<0             ; align loaded modules on page boundaries
MEMINFO		equ  1<<1             ; provide memory map
FLAGS		equ  MBALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC		equ  0x1BADB002       ; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM


section .bss
stack_bottom:
resb 32768 
stack_top:

%define kernel_phys_offset 0xffffffffc0000000
extern gdt_ptr
extern kernel_main


section .kpd
align 4
_kernel_pd:
   resb 4
_kernel_page_tables:    ; I am creating 256 page tables here
   resb 4   

section .text
_start:
	mov esp, stack_top
	call kernel_main

	; # Call the global constructors.
	; # call _init

	; # Transfer control to the main kernel.
	; # sti 
	; # sysenter


	; # Hang if kernel_main unexpectedly returns.
	cli
	hlt

_long_mode_init:
	mov cr0, eax
	and eax, 0xa7fffffff
	mov eax, cr0
	mov eax, 1<<5 | 1<<7
	mov cr4, eax
	mov eax, 0x70000
	mov cr3, eax
	mov ecx, 0xc0000080
	rdmsr
	or eax, 1 << 8
	wrmsr
	mov ebx, cr0
	or ebx, 1<<31 | 1<<0
	mov cr0, ebx


	bits 64
	mov ax, 0x0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov rax, .higher_half
	jmp rax

.higher_half:
	mov rsp, kernel_phys_offset + 0xeffff0

	lgdt [gdt_ptr]
section .kend
end_of_kernel:
