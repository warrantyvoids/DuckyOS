#ifndef ARCH_i686_APIC_H
#define ARCH_i686_APIC_H

/**
* APIC Structure
*
* For docs, see Intel 64 and IA-32 Architectures Software Developers Manual,
* Volume 3A System programming guide Part 1, section 10 (page 352)
*
* (C) 2014 - Chris de Jong / dducks
*
* Released under 2 clause BSD license
*/
#include <stdint.h>
#include <stdbool.h>

bool apic_is_available();

void apic_init();
void apic_send_eoi();


#define APIC_BASE 0xFEE0000
#define APIC_MSR 0x1B

typedef volatile struct {
// 0x0000 - 0x1F are reserved  
   uint32_t __reserved_0000[4];
   uint32_t __reserved_0010[4];
// 0x0020 Local APIC ID register
   uint32_t __reserved_0020 : 24,
            physical_id     :  4,
            __reserved_0023 :  4;
   uint32_t __reserved_0024[3];
            
// 0x0030 Local APIC Version register (RO)
   const
   uint32_t version : 8,
            __reserved_0031 : 8,
            max_lvt_entries : 8,
            __reserved_0033 : 8;
   uint32_t __reserved_0034[3];
   
// 0x0040 - Reserved
   uint32_t __reserved_0040[4];
// 0x0050 - Reserved
   uint32_t __reserved_0050[4];
// 0x0060 - Reserved
   uint32_t __reserved_0060[4];
// 0x0070 - Reserved
   uint32_t __reserved_0070[4];
// 0x0080 Task Priority Register
   struct {
     uint32_t priority : 8,
            __reserved_0081 : 24;
     uint32_t __reserved_0084[3];
   } tpr;
   
// 0x0090 Arbitration Priority Register
  const
  struct {
    uint32_t priority : 8,
          __reserved_0091 : 24;
    uint32_t __reserved_0094[3];
  } apr,
// 0x00A0 Processor Priority Register  
  ppr;
  
// 0x00B0 End of Interrupt Register
  uint32_t eoi;
  uint32_t __reserved_00B4[3];
  
// 0x00C0 Remote Read Register - not used in P4's / Xeon's
  const
  uint32_t __reserved_00C0[4];

// 0x00D0 Logical Destination Register
  uint32_t __reserved_00D0 : 24,
           logical_destination : 8;
  uint32_t __reserved_00D4[3];

// 0x00E0 Destination Format Register
  uint32_t __reserved_00E0 : 28,
          destination_format_model : 4;
  uint32_t __reserved_00E4[3];
  
// 0x00F0 Spurious Interrupt Vector Register
  uint32_t spurious_int_vector : 8,
           enabled : 1,
           focus_cpu : 1,        //Not supported on Intel Xeon!!! Probably shouldn't be defined here as well
           __reserved_00F1 : 2,
           eoi_broadcast_suppression : 1,
           __reserved_00F2 : 19;
  uint32_t __reserved_00F4[3];

// 0x0100 - 0x0170 Interrupt Status Register
  const
  struct {
    uint32_t bitfield;
    uint32_t __reserved_0104[3];
  } isr [8];
  
// 0x0180 - 0x01F0 Trigger Mode Register
  const
  struct {
    uint32_t bitfield;
    uint32_t __reserved_0184[3];
  } tmr [8];
  
// 0x0200 - 0x0270 Interrupt Request Register
  const
  struct {
    uint32_t bitfield;
    uint32_t __reserved_0200[3];
  } irr[8];
  
// 0x0280 Error Status Register
  const uint32_t error;
  const uint32_t __reserved_0284[3];
  
// 0x0290 - 0x2E0 Reserved
  uint32_t __reserved_0290[4*6];
  
// 0x02F0 LVT CMCI. I have literally no idea what this does since documentation is _VERY_ limited.
  uint32_t lvt_cmci;
  uint32_t __reserved_02F4[3];
  
// 0x0300 - 0x0310 Interrupt command
  struct {
    uint32_t vector            : 8,
             delivery_mode     : 3,
             destination_mode  : 1,
             delivery_status   : 1,
             __reserved_0301   : 1,
             level	       : 1,
             trigger           : 1,
             __reserved_0302   : 2,
             shorthand         : 2,
             __reserved_0303   :12;
     uint32_t __reserved_0304[3];
     uint32_t __reserved_0310 : 24,
             destination : 8;
     uint32_t __reserved_0314[3];
  } icr;

  struct {
    uint32_t vector		: 8,
             __reserved_0x321   : 4,
             delivery_status    : 1,
             __reserved_0x322   : 3,
             mask		: 1,
             mode		: 2,
             __reserved_0x323   : 13;
   uint32_t __reserved_0324[3];
 } lvt_timer;

// 0x0320 LVT Timer register
// 0x0330 LVT Thermal sensors register
// 0x0340 LVT Performance monitor register (might not be present in future)
// 0x0350 LVT INT0 register
// 0x0360 LVT INT1 register
// 0x0370 LVT Error register
  struct {
    uint32_t vector		: 8,
             delivery_mode      : 3, //Not on ERROR
             __reserved_0x321   : 1,
             delivery_status    : 1, 
             polarity           : 1, //Only on LINT0/1
             remote_irr         : 1, //Only on LINT0/1
             trigger		: 1, //Only on LINT0/1
             mask               : 1,
             __reserved_0322   : 15;
    uint32_t __reserved_0324[3];
  } lvt_therm, lvt_perfm, lvt_lint0, lvt_lint1, lvt_error;

// 0x0380 Initial Count Register (for Timer)
  uint32_t timer_initial;
  uint32_t __reserved_0384[3];

// 0x0390 Current Count Register (for Timer) RO
  const
  uint32_t timer_current;
  uint32_t __reserved_0394[3];
  
// 0x03A0 - 0x03D0 Reserved
  uint32_t __reserved_03A0[4*4];
  
// 0x03E0 Divide Configuration Register (for Timer)
  uint32_t divisor : 4,
          __reserved_03E0 : 28;
  uint32_t __reserved_03E4[3];
  
  uint32_t __reserved_03F0[4];
           
} __attribute__((packed)) local_apic_t;

enum {
  APIC_TIMER_ONESHOT = 0b00,
  APIC_TIMER_PERIODIC = 0b01,
  APIC_TIMER_DEADLINE = 0b10
};


#endif
