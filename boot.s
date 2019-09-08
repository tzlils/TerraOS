# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 
stack_top:


.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp

	# Call the global constructors.
	# call _init

	# Transfer control to the main kernel.
	# sti 
	pushl %ebx
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
	hlt

1:	hlt
	jmp 1b

.global __idt_default_handler
.type __idt_default_handler, @function
__idt_default_handler:
	pushal
	mov $0x20, %al
	mov $0x20, %dx
	out %al, %dx
	popal
	iretl


.global _set_idtr
.type _set_idtr, @function
_set_idtr:
	push %ebp
	movl %esp, %ebp

	lidt 0x10F0

	movl %ebp, %esp
	pop %ebp
	ret
	
.global _set_gdtr
.type _set_gdtr, @function
_set_gdtr:
	push %ebp
	movl %esp, %ebp

	lgdt 0x800

	movl %ebp, %esp
	pop %ebp
	ret

.global _reload_segments
.type _reload_segments, @function
_reload_segments:
	push %ebp
	movl %esp, %ebp

	push %eax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	pop %eax

	ljmp $0x8, $me
me:
	movl %ebp, %esp
	pop %ebp
	ret

	
.global halt
halt:
    cli
    hlt

.section .text
.global pause
.type pause @function
pause:
    hlt
    ret

.section .text
.global sys_cli
.type sys_cli @function
sys_cli:
    hlt
    ret


.section .text
.global sys_sti
.type sys_sti @function
sys_sti:
    hlt
    ret
.size _start, . - _start

.section .kend
.global end_of_kernel
end_of_kernel:
