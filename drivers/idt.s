%macro pushall 0
	push rax
	push rcx
	push rdx
	push rbx
	push rbp
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popall 0
	pop r15
	pop r14
	pop r13	
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	pop rdx
	pop rcx
	pop rax
%endmacro

extern idtp

section .text
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
global isr128

isr0:
	cli
	push byte 0    ; A normal ISR stub that pops a dummy error code to keep uniform stack frame
	push byte 0
	jmp isr_common_stub

;  1: Debug Exception
isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub
	
isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub

isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub

isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub

isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub

isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub

isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub
;  8: Double Fault Exception (With Error Code!)
isr8:
	cli
	push byte 8        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub

isr10:
	cli
	push byte 10        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr11:
	cli
	push byte 11        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr12:
	cli
	push byte 12        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr13:
	cli
	push byte 13        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr14:
	cli
	push byte 14        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub

isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub

isr17:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub

isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub

isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub

isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub

isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub

isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub

isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub

isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub

isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub

isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub

isr27:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub

isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub

isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub

isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub

isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub


isr_common_stub:
	pushall
	mov rdi, rsp   ; RDI=1st parameter

	cld
	; mov rax, fault_handler
	call rax
	popall

	add rsp, 16     ; Cleans up the pushed error code and pushed ISR number
	iretq           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

extern irq_handler
irq_common_stub:
	pushall
	mov rdi, rsp   ; RDI=1st parameter

	cld
	mov rax, irq_handler
	call rax

	popall
	add rsp, 16
	iretq

global __idt_default_handler
__idt_default_handler:
	pushall

	popall
	iretq
	