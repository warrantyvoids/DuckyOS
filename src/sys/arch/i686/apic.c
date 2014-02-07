#include "../arch.h"
#include "util.h"
#include "apic.h"
#include "paging.h"
#include "../../terminal.h"

bool apic_is_available() {
  return cpuid_test_feature(CPUID_FEAT_EDX_APIC);
}

static local_apic_t * apic;

void apic_init() {

  terminal_writestring("APIC_INIT.\n");

  apic = 0;
  if (!apic_is_available()) {
    return;
  }
  
  uint64_t msr = rdmsr(APIC_MSR);
  uint32_t base_addr = msr & 0xFFFFF000;
  terminal_writestring("APIC Base Address: ");
  terminal_writehex(base_addr);
  terminal_putchar('\n');
  
  apic = paging_map((void*)base_addr, PAGING_READWRITE | PAGING_CACHE_DISABLE);
  terminal_writestring("APIC Virt Address: ");
  terminal_writehex((uint32_t)apic);
  terminal_putchar('\n');
  terminal_writestring("APIC Version: ");
  terminal_writehexdigits(apic->version);
  terminal_putchar('\n');
  
  //Well, now we know where it's mapped, lets reset it.
  apic->destination_format_model = 0x0F;
  apic->logical_destination = 01;
  apic->lvt_timer.mask = 1; // <- disable ALL interrupts
  apic->lvt_therm.mask = 1;
  apic->lvt_lint0.mask = 1;
  apic->lvt_lint1.mask = 1;
  apic->lvt_error.mask = 1;
  
  apic->tpr.priority = 0;
  
  msr |= (1 << 11);
  wrmsr(APIC_MSR, msr);
  terminal_writestring("APIC MSR val: ");
  terminal_writehex(msr); terminal_writehex(msr>>32);
  terminal_putchar('\n');
  

  uint32_t volatile * apicraw = (uint32_t volatile *) apic;
  
//  apic->focus_cpu = 1;
  //apicraw[0x0F0 >> 2] = 0x100 | 39;
//  apicraw[0x320 >> 2] = 0x20000 | 34;
//  apicraw[0x380 >> 2] = 10000000;
  
  //We ready nauw.
  
  apic->spurious_int_vector = 39;
  apic->enabled = 1;
  apic->lvt_timer.vector = 32;
  apic->lvt_timer.mode = 1; // Periodic
  apic->divisor = 0b1011;
  apic->timer_initial = 1000000000;
  apic->lvt_timer.mask = 0; //GO!  
}

void apic_send_eoi() {
  apic->eoi = 0;
}