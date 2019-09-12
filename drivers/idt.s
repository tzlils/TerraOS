
global __idt_default_handler, _set_idtr
section .text
__idt_default_handler:
	pushal
	mov [0x20], al
	mov [0x20], dx
	; out al, dx
	popal
	iretl


_set_idtr:
	; push ebp
	mov esp, ebp

	; lidt 0x10f0

	mov ebp, esp
	; pop ebp
	ret