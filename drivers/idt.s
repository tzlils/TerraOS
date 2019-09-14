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
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

isr0:
	push byte 0    ; A normal ISR stub that pops a dummy error code to keep uniform stack frame
	push byte 0
	jmp isr_common_stub

;  1: Debug Exception
isr1:
	push byte 0
	push byte 1
	jmp isr_common_stub
	
isr2:
	push byte 0
	push byte 2
	jmp isr_common_stub

isr3:
	push byte 0
	push byte 3
	jmp isr_common_stub

isr4:
	push byte 0
	push byte 4
	jmp isr_common_stub

isr5:
	push byte 0
	push byte 5
	jmp isr_common_stub

isr6:
	push byte 0
	push byte 6
	jmp isr_common_stub

isr7:
	push byte 0
	push byte 7
	jmp isr_common_stub
;  8: Double Fault Exception (With Error Code!)
isr8:
	push byte 8        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr9:
	push byte 0
	push byte 9
	jmp isr_common_stub

isr10:
	push byte 10        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr11:
	push byte 11        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr12:
	push byte 12        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr13:
	push byte 13        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr14:
	push byte 14        ; Note that we DON'T push a value on the stack in this one!
				   ; It pushes one already! Use this type of stub for exceptions
				   ; that pop error codes!
	jmp isr_common_stub

isr15:
	push byte 0
	push byte 15
	jmp isr_common_stub

isr16:
	push byte 0
	push byte 16
	jmp isr_common_stub

isr17:
	push byte 0
	push byte 17
	jmp isr_common_stub

isr18:
	push byte 0
	push byte 18
	jmp isr_common_stub

isr19:
	push byte 0
	push byte 19
	jmp isr_common_stub

isr20:
	push byte 0
	push byte 20
	jmp isr_common_stub

isr21:
	push byte 0
	push byte 21
	jmp isr_common_stub

isr22:
	push byte 0
	push byte 22
	jmp isr_common_stub

isr23:
	push byte 0
	push byte 23
	jmp isr_common_stub

isr24:
	push byte 0
	push byte 24
	jmp isr_common_stub

isr25:
	push byte 0
	push byte 25
	jmp isr_common_stub

isr26:
	push byte 0
	push byte 26
	jmp isr_common_stub

isr27:
	push byte 0
	push byte 26
	jmp isr_common_stub

isr28:
	push byte 0
	push byte 28
	jmp isr_common_stub

isr29:
	push byte 0
	push byte 29
	jmp isr_common_stub

isr30:
	push byte 0
	push byte 30
	jmp isr_common_stub

isr31:
	push byte 0
	push byte 31
	jmp isr_common_stub

isr32:
	push byte 0
	push byte 31
	jmp isr_common_stub

irq0:
	push byte 0
	push byte 32
	jmp irq_common_stub

irq1:
	push byte 0
	push byte 33
	jmp irq_common_stub

irq2:
	push byte 0
	push byte 34
	jmp irq_common_stub

irq3:
	push byte 0
	push byte 35
	jmp irq_common_stub

irq4:
	push byte 0
	push byte 36
	jmp irq_common_stub

irq5:
	push byte 0
	push byte 37
	jmp irq_common_stub

irq6:
	push byte 0
	push byte 38
	jmp irq_common_stub

irq7:
	push byte 0
	push byte 39
	jmp irq_common_stub


irq8:
	push byte 0
	push byte 40
	jmp irq_common_stub

irq9:
	push byte 0
	push byte 41
	jmp irq_common_stub

irq10:
	push byte 0
	push byte 42
	jmp irq_common_stub

irq11:
	push byte 0
	push byte 43
	jmp irq_common_stub

irq12:
	push byte 0
	push byte 44
	jmp irq_common_stub

irq13:
	push byte 0
	push byte 45
	jmp irq_common_stub

irq14:
	push byte 0
	push byte 46
	jmp irq_common_stub

irq15:
	push byte 0
	push byte 47
	jmp irq_common_stub


extern isr_handler
isr_common_stub:
	pushall
	mov rdi, rsp   ; RDI=1st parameter

	cld
	mov rax, isr_handler
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
	