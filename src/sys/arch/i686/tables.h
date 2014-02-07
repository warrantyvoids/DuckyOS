#ifndef ARCH_i686_GDT_H
#define ARCH_i686_GDT_H

void gdt_flush();
void gdt_init();
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

void idt_init();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif