TARGET = $(notdir $(CURDIR))
# CHANGE BELOW:
#~ INSTALL_DIR = /Applications/Arduino.app/Contents/Resources/Java
INSTALL_DIR = /home/bkubicek/software/arduino-0022
#~ PORT = /dev/cu.usbserial*
PORT = /dev/ttyACM0

# Get these values from:
#     $(INSTALL_DIR)/hardware/boards.txt
#     (arduino-0022/hardware/arduino/boards.txt)
# The values below are for the "Arduino Duemilanove or Nano w/ ATmega328"
# now for "Arduino Mega 2560"
UPLOAD_SPEED = 115200
UPLOAD_PROTOCOL = stk500v2
BUILD_MCU = atmega2560
BUILD_F_CPU = 16000000L
TERM=bash

# getting undefined reference to `__cxa_pure_virtual'
#~ [http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1254180518 Arduino Forum - Makefile]
#~ http://www.arduino.cc/playground/OpenBSD/CLI
#~ [http://arduino.cc/forum/index.php?topic=52041.0 A "simple" makefile for Arduino]
#~ [http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1275488191 Arduino Forum - Configuring avr-gcc options in arduino IDE]
# found in /usr/lib/gcc/avr/4.3.5/cc1plus; fixed with -Wl,--gc-section

############################################################################
# Below here nothing should be changed...

ARDUINO = $(INSTALL_DIR)/hardware/arduino/cores/arduino
#
#~ AVR_TOOLS_PATH = $(INSTALL_DIR)/hardware/tools/avr/bin
# in Ubuntu, avr-gcc is installed separate;
# only avrdude comes with the IDE
AVR_TOOLS_PATH = /usr/bin
AVR_DUDE_PATH = $(INSTALL_DIR)/hardware/tools
#
SRC = $(ARDUINO)/pins_arduino.c $(ARDUINO)/wiring.c \
    $(ARDUINO)/wiring_analog.c $(ARDUINO)/wiring_digital.c \
    $(ARDUINO)/wiring_pulse.c \
    $(ARDUINO)/wiring_shift.c $(ARDUINO)/WInterrupts.c
# added applet/$(TARGET).cpp as in IDE 0022
CXXSRC = $(ARDUINO)/HardwareSerial.cpp $(ARDUINO)/WMath.cpp \
    $(ARDUINO)/Print.cpp \
    $(ARDUINO)/main.cpp
#    applet/$(TARGET).cpp # no need, having a rule now for applet/$(TARGET).cpp.o
# added main.cpp, as in 0022
FORMAT = ihex

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = 2

# Place -D or -U options here
#~ CDEFS = -DBUILD_F_CPU=$(BUILD_F_CPU)
#~ CXXDEFS = -DBUILD_F_CPU=$(BUILD_F_CPU)
# now called DF_CPU
CDEFS = -DF_CPU=$(BUILD_F_CPU) -DARDUINO=22
CXXDEFS = -DF_CPU=$(BUILD_F_CPU) -DARDUINO=22

# Place -I options here
CINCS = -I$(ARDUINO) -I$(INSTALL_DIR)/libraries/LiquidCrystal/ -I$(INSTALL_DIR)/libraries/EEPROM/
CXXINCS = -I$(ARDUINO)

# Compiler flag to set the C Standard level.
# c89 - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99 - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
# note that typically, IDE 0022 uses -w to suppress warnings (both in cpp and c)!
CWARN = -Wall
#~ CWARN = -w
#  "-Wstrict-prototypes" is valid for Ada/C/ObjC but not for C++:
CCWARN = -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

# to eliminate pins_ardiuno warnings:
# http://arduino.cc/pipermail/developers_arduino.cc/2010-December/004005.html

# [http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1254180518 Arduino Forum - Makefile]
#~ For building the objects files "-ffunction-sections -fdata-sections" was missing
#~ and the final avr-gcc call needs "-Wl,--gc-section".
CXSECTF = -fno-exceptions -ffunction-sections -fdata-sections
CFINALF = -Wl,--gc-section

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CCWARN) $(CSTANDARD) $(CEXTRA)
# added CWARN also to .cpp
CXXFLAGS = $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CXSECTF)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS = -lm

# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:applet/$(TARGET).hex
AVRDUDE_FLAGS = -V -F \
    -p $(BUILD_MCU) -P $(AVRDUDE_PORT) -c $(UPLOAD_PROTOCOL) \
    -b $(UPLOAD_SPEED) -C $(INSTALL_DIR)/hardware/tools/avrdude.conf
#    -b $(UPLOAD_SPEED) -C $(INSTALL_DIR)/hardware/tools/avr/etc/avrdude.conf

# Program settings
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
#~ AVRDUDE = $(AVR_TOOLS_PATH)/avrdude
AVRDUDE = $(AVR_DUDE_PATH)/avrdude
REMOVE = rm -f
MV = mv -f

# Define all object files.
# NOTE: obj files will be created in respective src directories (libraries or $(INSTALL_DIR));
#  make clean deletes them fine
# note that srcs are in libraries or other directories;
# $(CXXSRC:.cpp=.o) will cause obj files to be in same loc as src files
#~ OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)
# to change the output directory for object files;
# must change the obj list here!
# and then, match to corresponding rule somehow?
# or leave this - and parse in rule (auth automatic variable $(@F))?
# "Suffix Replacement"
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# added - OBJ list, transformed into applet/
OBJT = $(addprefix applet/,$(notdir $(OBJ)))
ALLSRC = $(SRC) $(CXXSRC) $(ASRC)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(BUILD_MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(BUILD_MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(BUILD_MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# depended libraries of .pde need to be added from
# $(INSTALL_DIR)/libraries (TODO: and/or ~/sketchbook/libraries)
# grep for 'include', test if exists, add...
# note: prefix "a real tab character" http://www.delorie.com/djgpp/doc/ug/larger/makefiles.html
# $$ to escape $ for shell;
# note: must NOT put comments # inside bash execution;
# those would get removed by make; making shell see "EOF in backquote substitution"
#		 echo $$ix ; \
# 'shell' twice - for each subprocess! Backtick doesn't get expanded?
GREPRES:=$(shell for ix in $(shell grep include $(TARGET).pde | sed 's/.*[<"]\(.*\).h[>"].*/\1/'); do \
		if [ -d $(INSTALL_DIR)/libraries/$$ix ] ; then \
			LINCS="$$LINCS -I$(INSTALL_DIR)/libraries/$$ix" ;\
		fi; \
	done; \
	echo $$LINCS)
# append includes:
CINCS += $(GREPRES)
CXXINCS += $(GREPRES)
# append library source .cpp files too (CXXSRC)
GREPRESB:=$(shell for ix in $(shell grep include $(TARGET).pde | sed 's/.*[<"]\(.*\).h[>"].*/\1/'); do \
		if [ -d $(INSTALL_DIR)/libraries/$$ix ] ; then \
			CPPSRCS="$$CPPSRCS $(INSTALL_DIR)/libraries/$$ix/*.cpp" ;\
		fi; \
	done; \
	echo $$CPPSRCS)
CXXSRC += $(GREPRESB)
# added - only CXX obj from libraries:
CXXLIBOBJ = $(GREPRESB:.cpp=.o)

# Default target.
all: applet_files build sizeafter

build: elf hex

applet_files: $(TARGET).pde
    # Here is the "preprocessing".
    # It creates a .cpp file based with the same name as the .pde file.
    # On top of the new .cpp file comes the WProgram.h header.
    # At the end there is a generic main() function attached.
    # Then the .cpp file will be compiled. Errors during compile will
    # refer to this new, automatically generated, file.
    # Not the original .pde file you actually edit...
	test -d applet || mkdir applet
    # @ supresses printout of the cmdline itself; so only the out of echo is printed
	@echo ALL OBJT: $(OBJT)
	@echo ALL CXXLIBOBJ: $(CXXLIBOBJ)
#	echo '#include "WProgram.h"' > applet/$(TARGET).cpp
	@echo "#include \"WProgram.h\"\nvoid setup();\nvoid loop();\n" > applet/$(TARGET).cpp
	cat $(TARGET).pde >> applet/$(TARGET).cpp
    # no more need to cat main.cpp (v0022) - now it is compiled in
#	cat $(ARDUINO)/main.cpp >> applet/$(TARGET).cpp


elf: applet/$(TARGET).elf
hex: applet/$(TARGET).hex
eep: applet/$(TARGET).eep
lss: applet/$(TARGET).lss
sym: applet/$(TARGET).sym

# Program the device.
upload: applet/$(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) applet/$(TARGET).hex
ELFSIZE = $(SIZE) applet/$(TARGET).elf
sizebefore:
	@if [ -f applet/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter:
	@if [ -f applet/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); echo; fi

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
    --change-section-address .data-0x800000 \
    --change-section-address .bss-0x800000 \
    --change-section-address .noinit-0x800000 \
    --change-section-address .eeprom-0x810000

coff: applet/$(TARGET).elf
	$(COFFCONVERT) -O coff-avr applet/$(TARGET).elf $(TARGET).cof

extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr applet/$(TARGET).elf $(TARGET).cof

.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
    -$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
    --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

# Link: create ELF output file from library.
# NOTE: applet/$(TARGET).cpp.o MUST BE BEFORE applet/core.a
#  in the dependency list, so its rule runs first!
applet/$(TARGET).elf: $(TARGET).pde applet/$(TARGET).cpp.o applet/core.a
#	$(CC) $(ALL_CFLAGS) -o $@ applet/$(TARGET).cpp -L. applet/core.a $(LDFLAGS)
# changed as in IDE v0022: link cpp obj files
	@echo $$(tput bold)$$(tput setaf 2) $(CC) $$(tput sgr0) $(ALL_CFLAGS) $(CFINALF) -o $@ applet/$(TARGET).cpp.o $(CXXOBJ) -L. applet/core.a $(LDFLAGS)
	@$(CC) $(ALL_CFLAGS) $(CFINALF) -o $@ applet/$(TARGET).cpp.o $(CXXOBJ) -L. applet/core.a $(LDFLAGS)

# added: cpp.o depends on cpp (and .pde which generates it)
# $< "first item in the dependencies list"; $@ "left side of the :"; $^ "right side of the :"
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
applet/$(TARGET).cpp.o: applet/$(TARGET).cpp
	@echo $$(tput bold) $(CXX) $$(tput sgr0) -c $(ALL_CXXFLAGS) $< -o $@
	@$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

#~ applet/core.a: $(OBJ)
#~	 @for i in $(OBJ); do echo $(AR) rcs applet/core.a $$i; $(AR) rcs applet/core.a $$i; done

applet/core.a: $(OBJT)
	 @for i in $(OBJT); do echo $(AR) rcs applet/core.a $$i; $(AR) rcs applet/core.a $$i; done

# iterate through OBJ to find the original location; then build depending on source extension
# TODO: add handling of assembler files
applet/%.o:
	@for iob in $(OBJ); do \
		if [ "`basename $$iob`" = "`basename $@`" ]; then \
			for ios in $(ALLSRC); do \
				if [ "$${iob%%.*}" = "$${ios%%.*}" ]; then \
					case $${ios##*.} in \
						"cpp") \
							echo "$$(tput bold)$$(tput setaf 1) $(CXX) $$(tput sgr0) -c $(ALL_CXXFLAGS) $$ios -o $@"; \
							$(CXX) -c $(ALL_CXXFLAGS) $$ios -o $@;; \
						"c") \
							echo "$$(tput bold)$$(tput setaf 1) $(CC) $$(tput sgr0) -c $(ALL_CFLAGS) $$ios -o $@"; \
							$(CC) -c $(ALL_CFLAGS) $$ios -o $@;; \
					esac; \
				fi; \
			done; \
		fi; \
	done;

#~ # Compile: create object files from C++ source files.
#~ .cpp.o:
#~	 $(CXX) -c $(ALL_CXXFLAGS) $< -o $@

#~ # Compile: create object files from C source files.
#~ .c.o:
#~	 $(CC) -c $(ALL_CFLAGS) $< -o $@

#~ # Compile: create assembler files from C source files.
#~ .c.s:
#~	 $(CC) -S $(ALL_CFLAGS) $< -o $@

#~ # Assemble: create object files from assembler source files.
#~ .S.o:
#~	 $(CC) -c $(ALL_ASFLAGS) $< -o $@

#~ # Automatic dependencies
#~ %.d: %.c
#~	 $(CC) -M $(ALL_CFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@

#~ %.d: %.cpp
#~	 $(CXX) -M $(ALL_CXXFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@

# Target: clean project.
clean:
	$(REMOVE) applet/$(TARGET).hex applet/$(TARGET).eep applet/$(TARGET).cof applet/$(TARGET).elf \
        applet/$(TARGET).map applet/$(TARGET).sym applet/$(TARGET).lss applet/core.a \
        $(OBJT) applet/$(TARGET).cpp.o \
        $(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)

.PHONY: all build elf hex eep lss sym program coff extcoff clean applet_files sizebefore sizeafter
