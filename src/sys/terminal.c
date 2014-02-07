#include <stdbool.h> /* C doesn't have booleans by default. */
#include <stddef.h>
#include <stdint.h>

#include "terminal.h"

uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
  
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c)
{
        if (c == '\n') {
          if (++terminal_row == VGA_HEIGHT) terminal_row = 0;
          terminal_column = 0;
          return;
        }
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if ( ++terminal_column == VGA_WIDTH )
	{
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_row = 0;
		}
	}
}
 
void terminal_writestring(const char* data)
{
	while (*data)
     	  terminal_putchar(*data++);
}

void terminal_writehexdigits(uint8_t digit) {
  uint8_t low = digit & 0x0F;
  uint8_t high = (digit & 0xF0) >> 4;
  if (high > 9) {
      terminal_putchar('A' + high - 10);
  } else {
      terminal_putchar('0' + high);
  }
  if (low > 9) {
      terminal_putchar('A' + low - 10);
  } else {
      terminal_putchar('0' + low );
  }
  
}

void terminal_writehex(uint32_t val) {
  terminal_writehexdigits( (val & 0xFF000000) >> 24 );
  terminal_writehexdigits( (val & 0x00FF0000) >> 16 );
  terminal_writehexdigits( (val & 0x0000FF00) >> 8  );
  terminal_writehexdigits( (val & 0x000000FF)       );
}

