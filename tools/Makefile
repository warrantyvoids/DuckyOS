BINUTIL_VERSION = 2.24
#GRUB_VERSION = 2.00
GRUB_VERSION = grub-2.02-beta2
FETCH = wget
BINUTILS_MIRROR = http://ftp.gnu.org/gnu/binutils/
#GRUB_MIRROR = http://ftp.gnu.org/gnu/grub/
GRUB_MIRROR = http://git.savannah.gnu.org/cgit/grub.git/snapshot/
ARCH ?= i686
ARCHS ?= i686 x86_64 arm

all:    build

x86:
	$(eval ARCH = i686)
	$(eval export ARCH)
	make build

x86_64:
	$(eval ARCH = x86_64)
	$(eval export ARCH)
	make build

arm:
	$(eval ARCH = arm)
	$(eval export ARCH)
	make build

build: bin/$(ARCH)-elf-ld grub-$(ARCH)/grub-mkrescue

bin/$(ARCH)-elf-ld:
	make extract-binutils
	rm -rf binutils-$(ARCH)
	mkdir binutils-$(ARCH)
	cd binutils-$(ARCH); \
	../binutils-$(BINUTIL_VERSION)/configure --target=$(ARCH)-elf --prefix=$(PWD) --disable-nls && \
	make && \
	make install

clean:
	rm -f binutils.tar.bz2 binutils.tar.bz2.sig binutils.tar
	rm -f grub.tar.gz grub.tar.gz.sig grub.tar
	rm -rf binutils-$(BINUTIL_VERSION)
	rm -rf grub-$(GRUB_VERSION)
	$(foreach ARG,$(ARCHS), rm -rf binutils-$(ARG) $(ARCH)-elf;)
#        |    The wonderfull people at grub managed to fuck this up as well.
#       \|/ 
#	$(foreach ARG,$(ARCHS), rm -rf grub-$(ARG);) 

clean-all: clean
	rm -rf bin share lib sbin etc
	
extract-binutils: binutils-$(BINUTIL_VERSION)

extract-grub: grub-$(GRUB_VERSION)

binutils-$(BINUTIL_VERSION): binutils.tar
	tar -xf binutils.tar

binutils.tar: binutils.tar.bz2
	bzip2 -dk binutils.tar.bz2

grub-$(ARCH)/grub-mkrescue: #<-- We can clean up the mess and still have grub working...
	make extract-grub
	cd grub-$(GRUB_VERSION); ./autogen.sh
	rm -rf grub-$(ARCH)
	mkdir grub-$(ARCH)
	cd grub-$(ARCH); \
	../grub-$(GRUB_VERSION)/configure --target=$(ARCH)-elf --prefix=$(PWD) --disable-nls && \
	make && \
	make install && \
	make install      #<-- This is needed since grub make install fails to create files the first time...

grub-$(GRUB_VERSION): grub.tar
	tar -xf grub.tar

	
grub.tar: grub.tar.gz
	cp grub.tar.gz grub.tar.gz.backup
	gzip -d grub.tar.gz
	mv grub.tar.gz.backup grub.tar.gz

fetch: binutils.tar.bz2  binutils.tar.bz2.sig verify

grub.tar.gz:
	$(FETCH) -O grub.tar.gz $(GRUB_MIRROR)grub-$(GRUB_VERSION).tar.gz

grub.tar.gz.sig:
	$(FETCH) -O grub.tar.gz.sig $(GRUB_MIRROR)grub-$(GRUB_VERSION).tar.gz.sig

binutils.tar.bz2:
	$(FETCH) -O binutils.tar.bz2 $(BINUTILS_MIRROR)binutils-$(BINUTIL_VERSION).tar.bz2
	
binutils.tar.bz2.sig:
	$(FETCH) -O binutils.tar.bz2.sig $(BINUTILS_MIRROR)binutils-$(BINUTIL_VERSION).tar.bz2.sig
	
verify: grub.tar.gz grub.tar.gz.sig binutils.tar.bz2 binutils.tar.bz2.sig
	gpg --recv-keys 4AE55E93 > /dev/null 2>&1
	gpg --verify binutils.tar.bz2.sig binutils.tar.bz2
	gpg --recv-keys E82E4209 > /dev/null 2>&1
	gpg --verify grub.tar.gz.sig grub.tar.gz
