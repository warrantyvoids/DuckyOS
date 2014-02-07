VOSDIR = ../VOS/
DOCDIR = doc/html/*

all:
	cd $(VOSDIR); make clean && make doc
	cp -R $(VOSDIR)/$(DOCDIR) ./doxygen

clean:
	rm -rf ./doxygen
	mkdir ./doxygen