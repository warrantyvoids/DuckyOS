#include "../arch.h"
#include "util.h"
#include "pic.h"


static isr_handler_t handlers[256];

void isr_init() {
  for (uint16_t i = 0; i < 256; i++) {
    handlers[i] = 0;
  }
}

void isr_set_handler( uint8_t num, isr_handler_t handler ) {
  handlers[num] = handler;
}

void isr_handler(isr_args_t args) {
  if (handlers[args.interrupt_id]) {
    handlers[args.interrupt_id](args);
  } else {
    unhandled_interrupt(args);
  }
}

void irq_handler(isr_args_t args) {
  if (args.interrupt_id >=  IRQ8 )
    outb( PIC2_CMD , PIC_CMD_RESET );
    
  outb( PIC1_CMD, PIC_CMD_RESET );
  
  if (handlers[args.interrupt_id]) {
    handlers[args.interrupt_id](args);
  } else {
    unhandled_interrupt(args);
  }
}
  
