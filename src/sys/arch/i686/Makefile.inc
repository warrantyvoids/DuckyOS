#Architecture specific assembly sources.
ASRCS +=	arch/i686/entry.s \
 		arch/i686/isr.s
#		arch/i686/entry.s

#Architecture specific C sources.
CSRCS +=	arch/i686/irq.c \
		arch/i686/tables.c \
		arch/i686/paging.c \
		arch/i686/arch.c \
		arch/i686/pic.c \
		arch/i686/pit.c \
		arch/i686/apic.c \
		arch/i686/drv/i8042.c \
		arch/i686/drv/pci/pci.c
