CROSSCOMPILE ?= ../../tools/gcc-4.8.1/bin/i686-pc-elf-
ARCH ?= i686

CC = clang
LD = ../../tools/bin/$(ARCH)-elf-ld
AS = clang

CFLAGS = -std=gnu11 -ffreestanding -g -O2 -Wall -Wextra -target $(ARCH)-pc-none-eabi -DARCH=$(ARCH)
LDFLAGS = -O2
ASFLAGS = -ffreestanding -nostdlib -target $(ARCH)-pc-none-eabi

ASRCS = 
CSRCS = kernel.c \
	terminal.c

include arch/$(ARCH)/Makefile.inc

COBJS = $(CSRCS:.c=.o)
AOBJS = $(ASRCS:.s=.o)

LDSCRIPT = kernel.ld

OUTPUT = VOS.bin

all: $(OUTPUT) pci/identifiers.h


pciupdate:
	wget -O pci/identifiers.h http://www.pcidatabase.com/pci_c_header.php

image: $(OUTPUT)

$(OUTPUT): $(COBJS) $(AOBJS)
	$(LD) -T $(LDSCRIPT) $(LDFLAGS) -o $(OUTPUT) $(COBJS) $(AOBJS)

.s.o:
	$(AS) -c $(ASFLAGS) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(COBJS) $(AOBJS) $(OUTPUT)
