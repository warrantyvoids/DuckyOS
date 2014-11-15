VERSION = 0.0.1
IMGNAME = VOS.iso

ARCHS = i686 x86_64 arm
ARCH ?= i686
ifeq ($(ARCH),i686)
	QEMU_ARCH = i386
else
	QEMU_ARCH = $(ARCH)
endif

export ARCH
export ARCHS
export QEMU_ARCH

.PHONY: all
.DEFAULT: all
all: toolchain VOS.iso

$(IMGNAME): img/boot/VOS.bin
	tools/grub-$(ARCH)/grub-mkrescue -o $(IMGNAME) img/
	
VOS.img:
	qemu-img create -f qcow2 VOS.img 2G
  
img/boot/VOS.bin: src/sys/VOS.bin
	cp src/sys/VOS.bin img/boot/VOS.bin

.PHONY: src/sys/VOS.bin
src/sys/VOS.bin:
	cd src/sys/; make image

.PHONY: doc
doc:
	doxygen doc/Doxyfile

toolchain:
	cd tools && make build
	
toolchain-clean:
	cd tools && make clean
	
toolchain-clean-all:
	cd tools && make clean-all

clean: toolchain-clean
	rm -f $(IMGNAME)
	rm -f img/boot/VOS.bin
	rm -rf doc/html doc/latex doc/man
	cd src/sys/; make clean

mkdiskimage: VOS.img
	

simulate: $(IMGNAME) VOS.img
	qemu-system-x86_64 -cdrom $(IMGNAME) -s -smp 2 -soundhw ac97 -drive file=VOS.img,index=0,media=disk,if=sd

buildkernel:
	cd src/sys; make all;

buildworld:
	