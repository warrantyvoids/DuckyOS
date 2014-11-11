CROSSCOMPILE ?= ../../tools/gcc-4.8.1/bin/i686-pc-elf-
ARCH ?= i686

CC = clang
CXX = clang++
LD = ../../tools/bin/$(ARCH)-elf-ld
AS = clang

CFLAGS = -std=gnu11 -ffreestanding -g -O2 -Wall -Wextra -target $(ARCH)-pc-none-eabi -DARCH=$(ARCH) -I$(PWD) -mno-sse -mno-mmx
CXXFLAGS = -std=c++11 -ffreestanding -g -O2 -Wall -Wextra -target $(ARCH)-pc-none-eabi -DARCH=$(ARCH) -I$(PWD) -mno-sse -mno-mmx -fno-exceptions
LDFLAGS = -O2
ASFLAGS = -ffreestanding -nostdlib -target $(ARCH)-pc-none-eabi

ASRCS = 
CSRCS = kernel.c \
	terminal.c \
	keyboard.c

CXXSRCS =

include arch/$(ARCH)/Makefile.inc

COBJS = $(CSRCS:.c=.o)
CXXOBJS = $(CXXSRCS:.cpp=.o)
AOBJS = $(ASRCS:.s=.o)

LDSCRIPT = kernel.ld

OUTPUT = VOS.bin

all: $(OUTPUT) pci/identifiers.h

pciupdate:
	wget -O pci/identifiers.h http://www.pcidatabase.com/pci_c_header.php

image: $(OUTPUT)

$(OUTPUT): $(COBJS) $(AOBJS) $(CXXOBJS)
	$(LD) -T $(LDSCRIPT) $(LDFLAGS) -o $(OUTPUT) $(COBJS) $(AOBJS) $(CXXOBJS)

.s.o:
	$(AS) -c $(ASFLAGS) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
	
.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(COBJS) $(AOBJS) $(CXXOBJS) $(OUTPUT)
