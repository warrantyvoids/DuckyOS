#ifndef __ARCH_ARCH_H
#define __ARCH_ARCH_H

#ifndef ARCH
#define ARCH i686
#endif

#define STRINGIZE2( x ) #x
#define STRINGIZE( x ) STRINGIZE2(x)
#define ARCH_ISR STRINGIZE(ARCH/isr.h)
#define ARCH_TMR STRINGIZE(ARCH/timer.h)
#define ARCH_UTIL STRINGIZE(ARCH/util.h)

#include ARCH_ISR
#include ARCH_TMR
#include ARCH_UTIL

#undef ARCH_ISR
#undef STRINGIZE
#undef STRINGIZE2

/** Initialize architecture specific code */
void arch_entry ( void );
void arch_init( void );
//Will be run after some initialization (timer handler etc)
void arch_postinit( void );

typedef void (*isr_handler_t)(isr_args_t);
void isr_set_handler( uint8_t id, isr_handler_t handler );
void unhandled_interrupt(isr_args_t);

#endif