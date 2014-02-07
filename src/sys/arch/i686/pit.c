#include "../arch.h"
#include "util.h"
#include "pic.h"
#include "pit.h"

#define PIT_FREQ 250

#define PIT_DIVISOR 1193180

#define PIT_BASE 0x40
#define PIT_CMD (PIT_BASE + 0x03)
#define PIT_CH0 (PIT_BASE + 0x00)
#define PIT_CH1 (PIT_BASE + 0x01)
#define PIT_CH2 (PIT_BASE + 0x02)

#define PIT_CMD_INIT 0x36


#if (((PIT_DIVISOR / PIT_FREQ) & 0xFFFF0000) != 0)
#error "Invalid PIT frequency!"
#endif

void pit_init() {
  uint16_t div = (uint16_t) (PIT_DIVISOR / PIT_FREQ);
  
  outb(PIT_CMD, PIT_CMD_INIT);
  outb(PIT_CH0, div & 0x00FF);
  outb(PIT_CH0, (div & 0xFF00) >> 8);
}