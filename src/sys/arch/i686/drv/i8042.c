#include "i8042.h"
#include <arch/arch.h>
#include <terminal.h>

#define i8042_DATA  0x60
#define i8042_STATUS 0x64
#define i8042_COMMAND 0x64

#define i8042_STATUS_OUTBUF (1 << 0) //Must be set before reading
#define i8042_STATUS_INBUF  (1 << 1) //Must be clear before writing
#define i8042_STATUS_SYSTEM (1 << 2) //Do not touch, should be set or we shouldn't be alive
#define i8042_STATUS_COMMAND (1 << 3) //Set on controller addressing, clear on device addressing
#define i8042_STATUS_TIMEOUT (1 << 6) //Timeout occurred
#define i8042_STATUS_PARITY (1 << 7)  //Parity error

#define i8042_CMD_TEST 0xAA
#define i8042_RSP_TEST_OKAY 0x55
#define i8042_CMD_DISABLE_A 0xAD
#define i8042_CMD_DISABLE_B 0xA7
#define i8042_CMD_ENABLE_A 0xAE
#define i8042_CMD_ENABLE_B 0xA8
#define i8042_CMD_READ(n) (0x20 + n)
#define i8042_CMD_WRITE(n) (0x60 + n)

#define i8042_CMD0_A_IRQEN (1 << 0) //IRQ enable for channel A
#define i8042_CMD0_B_IRQEN (1 << 1) //IRQ enable for channel B
#define i8042_CMD0_A_SYSFLAG (1 << 2) //Should be set. If not we shouldn't be alive.
#define i8042_CMD0_A_CLK (1 << 4)   //channel A clkenable
#define i8042_CMD0_B_CLK (1 << 5)   //channel B clkenable
#define i8042_CMD0_A_TRANSL (1 << 6) //Translation for channel a

inline static bool wait_tx() {
  uint32_t cntr = 0x2000;
  uint8_t status;
  while (cntr--) {
    status = inb(i8042_STATUS);
    if ((status & i8042_STATUS_INBUF) == 0)
      return true;
  }
  return false;
}

inline static bool wait_rx() {
  uint32_t cntr = 0x2000;
  uint8_t status;
  while (cntr--) {
    status = inb(i8042_STATUS);
    if ((status & i8042_STATUS_OUTBUF) != 0)
      return true;
  }
  return false;
}

static bool has_secondary_channel;

#define PRINT_EXIT(msg) {terminal_writestring(msg); return; }
#define TIMEOUT_EXIT() {terminal_writestring("Timeout occurred during PS/2 initialization.\n"); return; }

void i8042_init() {
  uint8_t in;  
  outb(i8042_COMMAND, i8042_CMD_DISABLE_A);
  
  if (!wait_tx())
    PRINT_EXIT("i8042 controller timed out.\n");
    
  outb(i8042_COMMAND, i8042_CMD_DISABLE_B);
  
  if (!wait_tx())
    PRINT_EXIT("i8042 controller timed out.\n");
  
  for (int i = 0; i < 256; i++)
    inb(i8042_DATA);            //Flush the data buffer.
  
  outb(i8042_COMMAND, i8042_CMD_READ(0));  //Read REG1
  if (!wait_rx())
    PRINT_EXIT("i8042 controller did not respond.\n");
  
  in = inb(i8042_DATA);    //Clear \/ these bits.
  in &= ~ ( i8042_CMD0_A_IRQEN | i8042_CMD0_B_IRQEN | i8042_CMD0_A_TRANSL );
//  has_secondary_channel = (in & i8042_CMD0_B_CLK) != 0;
  
  if (!wait_tx())  //And write em back
    PRINT_EXIT("i8042 controller is retarded.\n");
    
  outb(i8042_COMMAND, i8042_CMD_WRITE(0));
  if (!wait_tx())
    PRINT_EXIT("Mweh.");
  
  outb(i8042_DATA, in);
  
  if (!wait_tx())
    PRINT_EXIT("Damn.");
    
  terminal_writestring("So far so good for PS/2!\n");
  
  //Okay. So we're now in defined state. Lets perform some selftests.
  outb(i8042_COMMAND, i8042_CMD_TEST);
  if (!wait_rx())
    PRINT_EXIT("Failed during the self-test!\n");
    
  in = inb(i8042_DATA);
  if (in != i8042_RSP_TEST_OKAY)
    PRINT_EXIT("Self test failed!");
    
  terminal_writestring("Selftest completed.\n");
  
  //We're tested nauw. Gofix keyboard/mouse.
  //terminal_writestring(has_secondary_channel ? "Single channel controller.\n" : "Dual channel controller.\n");
  
  if (!wait_tx())
     TIMEOUT_EXIT();
  outb(i8042_COMMAND, i8042_CMD_ENABLE_B);
  
}