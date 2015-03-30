all: srcdir

srcdir:
	ln -s Marlin src

pde:
	rm src/Marlin.pde

build-firmware: srcdir pde
	ino build -m mega2560

.PHONY: clean

clean:
	rm -rf .build/
	rm -rf lib/
	rm src
