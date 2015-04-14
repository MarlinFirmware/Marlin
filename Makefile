all: depends

depends:
	head -26 .travis.yml
	@echo after solving dependancies, try : make build-firmware

link-srcdir:
	ln -s Marlin src

clean-pde:
	rm src/Marlin.pde

build-firmware: link-srcdir clean-pde
	ino build -m mega2560

.PHONY: clean

clean:
	rm -rf .build/
	rm -rf lib/
	rm src
