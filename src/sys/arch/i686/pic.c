#include "../arch.h"
#include "util.h"
#include "pic.h"

/**
* Initializes our interrupt controllers.
* 
* By default, the bios configures them to map 0x0 - 0x07, 0x8 0xf to interrupts 0x8 - 0xf, 0x70 - 0x77
* But we need to remap them for protected mode (INTs 0 - 0x1f are exceptions)..
* So we remap them.
*/
void pic_init() {
  //Start the initialization sequence...  
  outb( PIC1_CMD, 0x11 ); udelay(1);
  outb( PIC2_CMD, 0x11 ); udelay(1);
  
  //Ctrl base addresses
  outb( PIC1_DATA, IRQ_BASE ); udelay(1);
  outb( PIC2_DATA, IRQ_BASE + 8 ); udelay(1);
  
  // to master: mask
  // to slave: master id
  outb( PIC1_DATA, (1 << IRQ_INTERCONNECT) ); udelay(1);
  outb( PIC2_DATA, IRQ_INTERCONNECT ); udelay(1);
  
  // Legacy mode.
  outb( PIC1_DATA, 0x1 ); udelay(1);
  outb( PIC2_DATA, 0x1 ); udelay(1);
  
  // Disable all IRQs except cascade
  outb( PIC1_DATA, 0xFF ^ (1 << IRQ_INTERCONNECT) ); udelay(1);
  outb( PIC2_DATA, 0xFF ); udelay(1);
  
}

void pic_enable_line( uint8_t num ) {
  if ( num < 8 ) { //First PIC
    outb( PIC1_DATA, inb( PIC1_DATA ) & ~(1 << num));
  } else { //second pic
    outb( PIC2_DATA, inb( PIC2_DATA ) & ~(1 << (num - 8)));
  }
}


void pic_disable_line( uint8_t num ) {
  if ( num < 8 ) { //First PIC
    outb( PIC1_DATA, inb( PIC1_DATA ) | (1 << num));
  } else { //second pic
    outb( PIC2_DATA, inb( PIC2_DATA ) | (1 << (num - 8)));
  }
}
