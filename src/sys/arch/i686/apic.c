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

  uint16_t * screen = paging_map((void*)0xB8000, PAGING_READWRITE);
  terminal_writehex((uint32_t)screen);
  screen[60] = 'T';
  screen[61] = 'E';
  screen[62] = 'S';
  screen[63] = 'T';
  return;
  
  terminal_writestring("First MEMMAP!\n");

  apic = 0;
  if (apic_is_available()) {
    return;
  }
  
  uint64_t msr = rdmsr(APIC_MSR);
  uint32_t base_addr = msr & 0xFFFFF000;
  
  apic = paging_map((void*)base_addr, PAGING_READWRITE | PAGING_CACHE_DISABLE);
  
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
  wrmsr(msr, APIC_MSR);
  
  //We ready nauw.
  apic->spurious_int_vector = 39;
  apic->enabled = 1;
  apic->lvt_timer.vector = 34;
  apic->lvt_timer.mode = 1; // Periodic
  apic->divisor = 0b1010;
  //apic->lvt_timer.mask = 0; //GO!
  
}