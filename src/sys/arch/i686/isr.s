
.macro ISR_NO_ERRORCODE id=0
.global isr\id
isr\id:
	cli
	pushl $0
	pushl $\id
	jmp isr_common
.endm

.macro ISR_ERRORCODE id=0
.global isr\id
isr\id:
	cli
	pushl $\id
	jmp isr_common
.endm

.macro IRQ_HANDLER id=0,isr=0
.global irq\id
irq\id:
	cli
	pushl $0
	pushl $\isr
	jmp irq_common_pic
.global irq_a_\id
irq_a_\id:
	cli
	pushl $0
	pushl $\isr
	jmp irq_common_apic
.endm

// Implement handlers for all ISRs with processor exception codes
.irp id,0,1,2,3,4,5,6,7,9,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
ISR_NO_ERRORCODE \id
.endr

.irp id,8,10,11,12,13,14
ISR_ERRORCODE \id
.endr

IRQ_HANDLER 0, 32
IRQ_HANDLER 1, 33
IRQ_HANDLER 2, 34
IRQ_HANDLER 3, 35
IRQ_HANDLER 4, 36
IRQ_HANDLER 5, 37
IRQ_HANDLER 6, 38
IRQ_HANDLER 7, 39
IRQ_HANDLER 8, 40
IRQ_HANDLER 9, 41
IRQ_HANDLER 10, 42
IRQ_HANDLER 11, 43
IRQ_HANDLER 12, 44
IRQ_HANDLER 13, 45
IRQ_HANDLER 14, 46
IRQ_HANDLER 15, 47

.global idt_flush
.type idt_flush,@function
idt_flush:
	movl 4(%esp), %eax
	lidt (%eax)
	ret

.extern isr_handler
isr_common:
	pusha
	movw %ds, %ax
	pushl %eax
	
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	
	call isr_handler
	
	popl %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	popa
	addl $8, %esp
	sti
	iret

irq_common_pic:
	pusha
	movw %ds, %ax
	pushl %eax
	
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	
	call irq_handler_pic
	
	popl %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	popa
	addl $8, %esp
	sti
	iret

irq_common_apic:
	pusha
	movw %ds, %ax
	pushl %eax
	
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	
	call irq_handler_apic
	
	popl %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	popa
	addl $8, %esp
	sti
	iret
