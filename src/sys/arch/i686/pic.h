#ifndef ARCH_i686_PIC_H
#define ARCH_i686_PIC_H

//This should be divisible by 8, for some reason...
//Connection between master and slave PIC
#define IRQ_INTERCONNECT 0x02

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1
#define PIC_CMD_INIT 0x11
#define PIC_CMD_RESET 0x20


void pic_init();
void pic_enable_line( uint8_t num );
void pic_disable_line( uint8_t num );

#endif
