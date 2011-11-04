#
# Arduino 0022 Makefile 
# Uno with DOGS102 Shield
#
# written by olikraus@gmail.com
#
# Features:
#   - boards.txt is used to derive parameters
#   - All intermediate files are put into a separate directory (TMPDIRNAME)
#   - Simple use: Copy Makefile into the same directory of the .pde file
#
# Limitations:
#   - requires UNIX environment
#   - TMPDIRNAME must be subdirectory of the current directory.
#
# Targets
# 	all		build everything
#	upload	build and upload to arduino
#	clean	remove all temporary files (includes final hex file)
#
# History
#	001	28 Apr 2010	first  release
#	002  05 Oct 2010	added 'uno'
#

#=== user configuration ===
# All ...PATH variables must have a '/' at the end

# Board (and prozessor) information: see $(ARDUINO_PATH)hardware/arduino/boards.txt
# Some examples:
#	BOARD		DESCRIPTION
#	uno			Arduino Uno
#	atmega328	Arduino Duemilanove or Nano w/ ATmega328
#	diecimila		Arduino Diecimila, Duemilanove, or Nano w/ ATmega168
#	mega		Arduino Mega
#	mini			Arduino Mini
#	lilypad328	LilyPad Arduino w/ ATmega328  
BOARD:=mega

# additional (comma separated) defines 
# -DDOGM128_HW		board is connected to DOGM128 display
# -DDOGM132_HW		board is connected to DOGM132 display
# -DDOGS102_HW		board is connected to DOGS102 display
# -DDOG_REVERSE		180 degree rotation
# -DDOG_SPI_SW_ARDUINO	force SW shiftOut
DEFS=-DDOGS102_HW -DDOG_DOUBLE_MEMORY -DDOG_SPI_SW_ARDUINO

# The location where the avr tools (e.g. avr-gcc) are located. Requires a '/' at the end.
# Can be empty if all tools are accessable through the search path
AVR_TOOLS_PATH:=/usr/bin/

# Install path of the arduino software. Requires a '/' at the end.
ARDUINO_PATH:=/home/bkubicek/software/arduino-0022/

# Install path for avrdude. Requires a '/' at the end. Can be empty if avrdude is in the search path.
AVRDUDE_PATH:= 

# The unix device where we can reach the arduino board
# Uno: /dev/ttyACM0
# Duemilanove: /dev/ttyUSB0
AVRDUDE_PORT:=/dev/ttyACM0

# List of all libaries which should be included.
#EXTRA_DIRS=$(ARDUINO_PATH)libraries/LiquidCrystal/
#EXTRA_DIRS+=$(ARDUINO_PATH)libraries/Dogm/
#EXTRA_DIRS+=/home/kraus/src/arduino/dogm128/hg/libraries/Dogm/

#=== fetch parameter from boards.txt processor parameter ===
# the basic idea is to get most of the information from boards.txt

BOARDS_TXT:=$(ARDUINO_PATH)hardware/arduino/boards.txt

# get the MCU value from the $(BOARD).build.mcu variable. For the atmega328 board this is atmega328p
MCU:=$(shell sed -n -e "s/$(BOARD).build.mcu=\(.*\)/\1/p" $(BOARDS_TXT))
# get the F_CPU value from the $(BOARD).build.f_cpu variable. For the atmega328 board this is 16000000
F_CPU:=$(shell sed -n -e "s/$(BOARD).build.f_cpu=\(.*\)/\1/p" $(BOARDS_TXT))

# avrdude
# get the AVRDUDE_UPLOAD_RATE value from the $(BOARD).upload.speed variable. For the atmega328 board this is 57600
AVRDUDE_UPLOAD_RATE:=$(shell sed -n -e "s/$(BOARD).upload.speed=\(.*\)/\1/p" $(BOARDS_TXT))
# get the AVRDUDE_PROGRAMMER value from the $(BOARD).upload.protocol variable. For the atmega328 board this is stk500
# AVRDUDE_PROGRAMMER:=$(shell sed -n -e "s/$(BOARD).upload.protocol=\(.*\)/\1/p" $(BOARDS_TXT))
# use stk500v1, because stk500 will default to stk500v2
AVRDUDE_PROGRAMMER:=stk500v1

#=== identify user files ===
PDESRC:=$(shell ls *.pde)
TARGETNAME=$(basename $(PDESRC))

CDIRS:=$(EXTRA_DIRS) $(addsuffix utility/,$(EXTRA_DIRS))
CDIRS:=*.c utility/*.c $(addsuffix *.c,$(CDIRS)) $(ARDUINO_PATH)hardware/arduino/cores/arduino/*.c
CSRC:=$(shell ls $(CDIRS) 2>/dev/null)

CCSRC:=$(shell ls *.cc 2>/dev/null)

CPPDIRS:=$(EXTRA_DIRS) $(addsuffix utility/,$(EXTRA_DIRS))
CPPDIRS:=*.cpp utility/*.cpp $(addsuffix *.cpp,$(CPPDIRS)) $(ARDUINO_PATH)hardware/arduino/cores/arduino/*.cpp 
CPPSRC:=$(shell ls $(CPPDIRS) 2>/dev/null)

#=== build internal variables ===

# the name of the subdirectory where everything is stored
TMPDIRNAME:=tmp
TMPDIRPATH:=$(TMPDIRNAME)/

AVRTOOLSPATH:=$(AVR_TOOLS_PATH)

OBJCOPY:=$(AVRTOOLSPATH)avr-objcopy
OBJDUMP:=$(AVRTOOLSPATH)avr-objdump
SIZE:=$(AVRTOOLSPATH)avr-size

CPPSRC:=$(addprefix $(TMPDIRPATH),$(PDESRC:.pde=.cpp)) $(CPPSRC)

COBJ:=$(CSRC:.c=.o)
CCOBJ:=$(CCSRC:.cc=.o)
CPPOBJ:=$(CPPSRC:.cpp=.o)

OBJFILES:=$(COBJ)  $(CCOBJ)  $(CPPOBJ)
DIRS:= $(dir $(OBJFILES))

DEPFILES:=$(OBJFILES:.o=.d)
# assembler files from avr-gcc -S
ASSFILES:=$(OBJFILES:.o=.s)
# disassembled object files with avr-objdump -S
DISFILES:=$(OBJFILES:.o=.dis)


LIBNAME:=$(TMPDIRPATH)$(TARGETNAME).a
ELFNAME:=$(TMPDIRPATH)$(TARGETNAME).elf
HEXNAME:=$(TMPDIRPATH)$(TARGETNAME).hex

AVRDUDE_FLAGS = -V -F
AVRDUDE_FLAGS += -C $(ARDUINO_PATH)/hardware/tools/avrdude.conf 
AVRDUDE_FLAGS += -p $(MCU)
AVRDUDE_FLAGS += -P $(AVRDUDE_PORT)
AVRDUDE_FLAGS += -c $(AVRDUDE_PROGRAMMER) 
AVRDUDE_FLAGS += -b $(AVRDUDE_UPLOAD_RATE)
AVRDUDE_FLAGS += -U flash:w:$(HEXNAME)

AVRDUDE = avrdude

#=== predefined variable override ===
# use "make -p -f/dev/null" to see the default rules and definitions

# Build C and C++ flags. Include path information must be placed here
COMMON_FLAGS = -DF_CPU=$(F_CPU) -mmcu=$(MCU) $(DEFS)
# COMMON_FLAGS += -gdwarf-2
COMMON_FLAGS += -Os
COMMON_FLAGS += -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
COMMON_FLAGS += -I. 
COMMON_FLAGS += -I$(ARDUINO_PATH)hardware/arduino/cores/arduino
COMMON_FLAGS += $(addprefix -I,$(EXTRA_DIRS))
COMMON_FLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
COMMON_FLAGS += -Wl,--relax
COMMON_FLAGS += -mcall-prologues

CFLAGS = $(COMMON_FLAGS) -std=gnu99 -Wstrict-prototypes  
CXXFLAGS = $(COMMON_FLAGS) 

# Replace standard build tools by avr tools
CC = $(AVRTOOLSPATH)avr-gcc
CXX = $(AVRTOOLSPATH)avr-g++
AR  = @$(AVRTOOLSPATH)avr-ar


# "rm" must be able to delete a directory tree
RM = rm -rf 

#=== rules ===

# add rules for the C/C++ files where the .o file is placed in the TMPDIRPATH
# reuse existing variables as far as possible

$(TMPDIRPATH)%.o: %.c
	@echo compile $<
	@$(COMPILE.c) $(OUTPUT_OPTION) $<

$(TMPDIRPATH)%.o: %.cc
	@echo compile $< 
	@$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(TMPDIRPATH)%.o: %.cpp
	@echo compile $<
	@$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(TMPDIRPATH)%.s: %.c
	@$(COMPILE.c) $(OUTPUT_OPTION) -S $<

$(TMPDIRPATH)%.s: %.cc
	@$(COMPILE.cc) $(OUTPUT_OPTION) -S $<

$(TMPDIRPATH)%.s: %.cpp
	@$(COMPILE.cpp) $(OUTPUT_OPTION) -S $<

$(TMPDIRPATH)%.dis: $(TMPDIRPATH)%.o
	@$(OBJDUMP) -S $< > $@

.SUFFIXES: .elf .hex .pde

.elf.hex:
	@$(OBJCOPY) -O ihex -R .eeprom $< $@
	
$(TMPDIRPATH)%.cpp: %.pde
	@cat $(ARDUINO_PATH)hardware/arduino/cores/arduino/main.cpp > $@
	@cat $< >> $@
	@echo >> $@
	@echo 'extern "C" void __cxa_pure_virtual() { while (1); }' >> $@


.PHONY: all
all: tmpdir $(HEXNAME) assemblersource showsize
	ls -al $(HEXNAME) $(ELFNAME)

$(ELFNAME): $(LIBNAME)($(addprefix $(TMPDIRPATH),$(OBJFILES))) 
	$(LINK.o) $(COMMON_FLAGS) $(LIBNAME) $(LOADLIBES) $(LDLIBS) -o $@

$(LIBNAME)(): $(addprefix $(TMPDIRPATH),$(OBJFILES))

#=== create temp directory ===
# not really required, because it will be also created during the dependency handling
.PHONY: tmpdir
tmpdir:
	@test -d $(TMPDIRPATH) || mkdir $(TMPDIRPATH)

#=== create assembler files for each C/C++ file ===
.PHONY: assemblersource
assemblersource: $(addprefix $(TMPDIRPATH),$(ASSFILES)) $(addprefix $(TMPDIRPATH),$(DISFILES))


#=== show the section sizes of the ELF file ===
.PHONY: showsize
showsize: $(ELFNAME)
	$(SIZE) $<

#=== clean up target ===
# this is simple: the TMPDIRPATH is removed
.PHONY: clean
clean:
	$(RM) $(TMPDIRPATH)

# Program the device.  
# step 1: reset the arduino board with the stty command
# step 2: user avrdude to upload the software
.PHONY: upload
upload: $(HEXNAME)
	stty -F $(AVRDUDE_PORT) hupcl
	$(AVRDUDE) $(AVRDUDE_FLAGS)


# === dependency handling ===
# From the gnu make manual (section 4.14, Generating Prerequisites Automatically)
# Additionally (because this will be the first executed rule) TMPDIRPATH is created here.
# Instead of "sed" the "echo" command is used
# cd $(TMPDIRPATH); mkdir -p $(DIRS) 2> /dev/null; cd ..
DEPACTION=test -d $(TMPDIRPATH) || mkdir $(TMPDIRPATH);\
mkdir -p $(addprefix $(TMPDIRPATH),$(DIRS));\
set -e; echo -n $@ $(dir $@) > $@; $(CC) -MM $(COMMON_FLAGS) $< >> $@


$(TMPDIRPATH)%.d: %.c
	@$(DEPACTION)

$(TMPDIRPATH)%.d: %.cc
	@$(DEPACTION)


$(TMPDIRPATH)%.d: %.cpp
	@$(DEPACTION)

# Include dependency files. If a .d file is missing, a warning is created and the .d file is created
# This warning is not a problem (gnu make manual, section 3.3 Including Other Makefiles)
-include $(addprefix $(TMPDIRPATH),$(DEPFILES))


