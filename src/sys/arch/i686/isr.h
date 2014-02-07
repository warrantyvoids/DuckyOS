#ifndef ARCH_i686_ISR_H
#define ARCH_i686_ISR_H

#include <stdint.h>

#define IRQ_BASE 32

#define IRQ0 (IRQ_BASE + 0)
#define IRQ1 (IRQ_BASE + 1)
#define IRQ2 (IRQ_BASE + 2)
#define IRQ3 (IRQ_BASE + 3)
#define IRQ4 (IRQ_BASE + 4)
#define IRQ5 (IRQ_BASE + 5)
#define IRQ6 (IRQ_BASE + 6)
#define IRQ7 (IRQ_BASE + 7)
#define IRQ8 (IRQ_BASE + 8)
#define IRQ9 (IRQ_BASE + 9)
#define IRQ10 (IRQ_BASE + 10)
#define IRQ11 (IRQ_BASE + 11)
#define IRQ12 (IRQ_BASE + 12)
#define IRQ13 (IRQ_BASE + 13)
#define IRQ14 (IRQ_BASE + 14)
#define IRQ15 (IRQ_BASE + 15)


typedef struct {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, arg2, arg4, arg3, arg1;
  uint32_t interrupt_id, error_code;
  uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed)) isr_args_t;

void isr_init();

inline static void isr_enable() {
  asm volatile (" sti ");
}
inline static void isr_disable() {
  asm volatile (" cli ");
}


#endif