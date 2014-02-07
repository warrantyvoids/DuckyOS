// Defines the structures of a GDT entry and of a GDT pointer
#include <stdint.h>
#include "tables.h"
#include "isr.h"
#include "irqdefs.h"

//Todo: Fix this. C11 alignas?
struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));
 
struct gdt_ptr
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct idt_entry
{
	uint16_t base_low;
	uint16_t selector;
	uint8_t ignore_is_always_0;
	uint8_t flags;
	uint16_t base_high;
}  __attribute__((packed));

struct idt_ptr
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
// We'll need at least 5 entries in our GDT...
 
struct gdt_entry gdt[5];
struct gdt_ptr p_gdt;
struct idt_entry idts[256];
struct idt_ptr p_idt;
 
// Very simple: fills a GDT entry using the parameters
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
 
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);
 
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}
 
// Sets our 5 gates and installs the real GDT through the assembler function
void gdt_init()
{
	p_gdt.limit = (sizeof(struct gdt_entry) * 3) - 1;
	p_gdt.base = (unsigned int)&gdt;
 
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
 
	gdt_flush();
}

void idt_flush(uint32_t);

void idt_init() {
  	p_idt.limit = sizeof(struct idt_entry) * 256 - 1;
  	p_idt.base = (uint32_t) &idts;
  	
  	for (uint32_t i = 0; i < sizeof(struct idt_entry) * 256; i++)
  		((uint8_t*)&idts)[i] = 0x00;
  	
  	idt_set_gate( 0, (uint32_t) &isr0, 0x08, 0x8E);
  	idt_set_gate( 1, (uint32_t) &isr1, 0x08, 0x8E);
  	idt_set_gate( 2, (uint32_t) &isr2, 0x08, 0x8E);
  	idt_set_gate( 3, (uint32_t) &isr3, 0x08, 0x8E);
  	idt_set_gate( 4, (uint32_t) &isr4, 0x08, 0x8E);
  	idt_set_gate( 5, (uint32_t) &isr5, 0x08, 0x8E);
  	idt_set_gate( 6, (uint32_t) &isr6, 0x08, 0x8E);
  	idt_set_gate( 7, (uint32_t) &isr7, 0x08, 0x8E);
  	idt_set_gate( 8, (uint32_t) &isr8, 0x08, 0x8E);
  	idt_set_gate( 9, (uint32_t) &isr9, 0x08, 0x8E);
  	idt_set_gate( 10, (uint32_t) &isr10, 0x08, 0x8E);
  	idt_set_gate( 11, (uint32_t) &isr11, 0x08, 0x8E);
  	idt_set_gate( 12, (uint32_t) &isr12, 0x08, 0x8E);
  	idt_set_gate( 13, (uint32_t) &isr13, 0x08, 0x8E);
  	idt_set_gate( 14, (uint32_t) &isr14, 0x08, 0x8E);
  	idt_set_gate( 15, (uint32_t) &isr15, 0x08, 0x8E);
  	idt_set_gate( 16, (uint32_t) &isr16, 0x08, 0x8E);
  	idt_set_gate( 17, (uint32_t) &isr17, 0x08, 0x8E);
  	idt_set_gate( 18, (uint32_t) &isr18, 0x08, 0x8E);
  	idt_set_gate( 19, (uint32_t) &isr19, 0x08, 0x8E);
  	idt_set_gate( 20, (uint32_t) &isr20, 0x08, 0x8E);
  	idt_set_gate( 21, (uint32_t) &isr21, 0x08, 0x8E);
  	idt_set_gate( 22, (uint32_t) &isr22, 0x08, 0x8E);
  	idt_set_gate( 23, (uint32_t) &isr23, 0x08, 0x8E);
  	idt_set_gate( 24, (uint32_t) &isr24, 0x08, 0x8E);
  	idt_set_gate( 25, (uint32_t) &isr25, 0x08, 0x8E);
  	idt_set_gate( 26, (uint32_t) &isr26, 0x08, 0x8E);
  	idt_set_gate( 27, (uint32_t) &isr27, 0x08, 0x8E);
  	idt_set_gate( 28, (uint32_t) &isr28, 0x08, 0x8E);
  	idt_set_gate( 29, (uint32_t) &isr29, 0x08, 0x8E);
  	idt_set_gate( 30, (uint32_t) &isr30, 0x08, 0x8E);
  	idt_set_gate( 31, (uint32_t) &isr31, 0x08, 0x8E);

  	idt_set_gate( IRQ0, (uint32_t) &irq0, 0x08, 0x8E);
  	idt_set_gate( IRQ1, (uint32_t) &irq1, 0x08, 0x8E);
  	idt_set_gate( IRQ2, (uint32_t) &irq2, 0x08, 0x8E);
  	idt_set_gate( IRQ3, (uint32_t) &irq3, 0x08, 0x8E);
  	idt_set_gate( IRQ4, (uint32_t) &irq4, 0x08, 0x8E);
  	idt_set_gate( IRQ5, (uint32_t) &irq5, 0x08, 0x8E);
  	idt_set_gate( IRQ6, (uint32_t) &irq6, 0x08, 0x8E);
  	idt_set_gate( IRQ7, (uint32_t) &irq7, 0x08, 0x8E);
  	idt_set_gate( IRQ8, (uint32_t) &irq8, 0x08, 0x8E);
  	idt_set_gate( IRQ9, (uint32_t) &irq9, 0x08, 0x8E);
  	idt_set_gate( IRQ10, (uint32_t) &irq10, 0x08, 0x8E);
  	idt_set_gate( IRQ11, (uint32_t) &irq11, 0x08, 0x8E);
  	idt_set_gate( IRQ12, (uint32_t) &irq12, 0x08, 0x8E);
  	idt_set_gate( IRQ13, (uint32_t) &irq13, 0x08, 0x8E);
  	idt_set_gate( IRQ14, (uint32_t) &irq14, 0x08, 0x8E);
  	idt_set_gate( IRQ15, (uint32_t) &irq15, 0x08, 0x8E);

  	idt_flush( (uint32_t) &p_idt);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
	idts[num].base_low = base & 0xFFFF;
	idts[num].base_high = (base >> 16) & 0xFFFF;
	
	idts[num].selector = selector;
	idts[num].ignore_is_always_0 = 0;
	idts[num].flags = flags; /* | 0x60; <-- For privilige switching */ 
}

