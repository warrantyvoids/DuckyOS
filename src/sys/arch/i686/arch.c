#include "../arch.h"
#include "apic.h"
#include "pic.h"
#include "pit.h"
#include "tables.h"
#include "paging.h"

void arch_entry() { 
  paging_init();
  gdt_init();
  idt_init();
  isr_init();
  pic_init();
  pit_init();
}

void arch_init() {
  if (apic_is_available()) {
    apic_init();
  } else {
//    pic_init();
//    pit_init();
  }

}

void arch_postinit() {

//  pic_enable_line(0);
}