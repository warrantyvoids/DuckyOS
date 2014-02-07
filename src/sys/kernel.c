#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "terminal.h"
#include "arch/arch.h"
#include "arch/i686/apic.h"
 
/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

static void protection_fault( isr_args_t args ) {
  terminal_writestring("General protection fault! type = ");
  terminal_writehexdigits( args.error_code );
  terminal_putchar('\n');
  while (1);
}

static void double_fault ( isr_args_t args ) {
  terminal_writestring("A double fault has occurred while serving int ");
  terminal_writehexdigits( args.error_code );
  terminal_putchar('\n');
}

void unhandled_interrupt( isr_args_t args ) {
  terminal_writestring("Unhandled interrupt. Type = ");
  terminal_writehexdigits( args.interrupt_id );
  terminal_putchar('\n');
}

static void int_hang ( isr_args_t args ) {
  terminal_writestring("Stopping. Recieved int ");
  terminal_writehexdigits(args.interrupt_id);
  terminal_writestring(" with error code ");
  terminal_writehex(args.error_code);
  terminal_writestring(".\n");
  
  while (1);
}

static void timer_interrupt( isr_args_t args ) {
  static uint32_t timerval = 0;
  terminal_writestring("Timer ");
  terminal_writehex(timerval++);
  terminal_putchar('\n');
}
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main()
{
        arch_entry();

	terminal_initialize();
	terminal_writestring("Kernel initialized.\n");
	terminal_writestring("VGA Enabled!\n");
     
//        isr_set_handler(3, &timer_interrupt);        
        isr_set_handler(IRQ_TIMER, &timer_interrupt);
        isr_set_handler(0x0D, &protection_fault);
        isr_set_handler(0x08, &double_fault);
        isr_set_handler(0x06, &int_hang );

        terminal_writestring("ISR Initialized...\n");

        isr_enable();
        
        arch_init();

        terminal_writestring("Init survived...\n");
        
        arch_postinit();
        
	/* Since there is no support for newlines in terminal_putchar yet, \n will
	   produce some VGA specific character instead. This is normal. */

	char cpuvend [14];
        cpuvend[12] = '\n';
        cpuvend[13] = '\0';        
        
        cpuid_vendor(cpuvend);
        terminal_writestring(cpuvend);

        if (apic_is_available()) {
          terminal_writestring("ACPI Available!\n");
        } else {
          terminal_writestring("No ACPI!\n");
        }
        

//	asm volatile (" int $0x01 ");
//	asm volatile (" int $0x04 ");
	
//	uint32_t rax, rdx;
//	asm volatile (" rdmsr " : "=a"(rax), "=d"(rdx) : "c"(0x1B) );
	
//	terminal_writestring("RDMSR 0x1B:\n");
//	terminal_writehex(rax); terminal_putchar('\n');
//	terminal_writehex(rdx); terminal_putchar('\n');
		
	while (1);
}
