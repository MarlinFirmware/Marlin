#
# This makefile allows you to build sketches from the command line
# without the Arduino environment (or Java).
#
# Detailed instructions for using the makefile:
#
#  1. Modify the line containg "ARDUINO_INSTALL_DIR" to point to the directory
#     that contains the Arduino installation (for example, under Mac OS X, this
#     might be /Applications/Arduino.app/Contents/Resources/Java).
#
#  2. Modify the line containing "UPLOAD_PORT" to refer to the filename
#     representing the USB or serial connection to your Arduino board
#     (e.g. UPLOAD_PORT = /dev/tty.USB0).  If the exact name of this file
#     changes, you can use * as a wildcard (e.g. UPLOAD_PORT = /dev/tty.usb*).
#
#  3. Type "make <printer>" and press enter to load the <printer> machine 
#     configuration.
#
#  4. Type "make" and press enter to compile the program for the current
#     configuration.
#
#  5. Type "make upload", reset your Arduino board, and press enter to
#     upload your program to the Arduino board.
#


#
# User configuration: 
#

# Arduino source install directory and Arduino SDK version:
#ARDUINO_INSTALL_DIR           =

# Directory used to build files in. It contains all the build files, from
# object files to the final hex file:
#BUILD_DIR                     =

# Programmer configuration:
#UPLOAD_PORT                   =

############################################################################
# Below here nothing should be changed.

# Check platform:
ifdef ComSpec
	OS_TYPE = WIN
else
	OS_TYPE = UNIX
endif

# Set Makefile platform-dependencies:
ifeq ($(OS_TYPE), WIN)
	PATHSEP_TMP=\\
else
	PATHSEP_TMP=/
endif
PATHSEP = $(strip $(PATHSEP_TMP))

#
# Check user configuration:
#

# If there are any user-configured variable not defined, set a default value.
ifeq ($(OS_TYPE), WIN)
	ARDUINO_INSTALL_DIR ?= C:\Arduino
else
ifndef ARDUINO
	ARDUINO_INSTALL_DIR ?= ${HOME}/arduino
else
	ARDUINO_INSTALL_DIR ?= ${ARDUINO}
endif
endif
space :=
space +=

ARDUINO_VERSION = 164

BUILD_DIR ?= bin

AVRDUDE_PROGRAMMER = wiring
UPLOAD_RATE = 115200
ifeq ($(OS_TYPE), WIN)
	UPLOAD_PORT ?= COM$(COM)
else
	UPLOAD_PORT ?= /dev/ttyUSB*
endif


#
# Build Settings:
#

# Language:
DEFAULT_LANGUAGE = EN
LANGUAGE ?= $(shell echo $(DEFAULT_LANGUAGE) | tr a-z A-Z)
# Main Makefile
MAKEFILE = Makefile
# Output format
FORMAT = ihex
# Output file
TARGET = $(notdir $(CURDIR))

# Verbose make
ifndef LOG
	LOG=no
endif

ifeq ($(LOG), no)
	L = @
endif

# AVR tools path:
AVR_TOOLS_PATH = $(ARDUINO_INSTALL_DIR)$(PATHSEP)hardware$(PATHSEP)tools$(PATHSEP)avr$(PATHSEP)bin
# AVR tool binaries:
OBJCOPY = $(AVR_TOOLS_PATH)$(PATHSEP)avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)$(PATHSEP)avr-objdump
AR  = $(AVR_TOOLS_PATH)$(PATHSEP)avr-ar
SIZE = $(AVR_TOOLS_PATH)$(PATHSEP)avr-size
NM = $(AVR_TOOLS_PATH)$(PATHSEP)avr-nm
AVRDUDE = $(AVR_TOOLS_PATH)$(PATHSEP)avrdude
# Other tools:
ifeq ($(OS_TYPE), WIN)
	MOVE = move
	COPY = copy
	REMOVE = del /F /Q
	MAKE_DIR = mkdir
	REMOVE_DIR = del /F /Q
	ECHO = echo
else
	MAKE_DIR = mkdir -p
	MOVE = mv -f
	COPY = cp
	REMOVE = rm -f
	REMOVE_DIR = rm -fr
	ECHO = echo
endif

CC = $(AVR_TOOLS_PATH)$(PATHSEP)avr-gcc
CXX = $(AVR_TOOLS_PATH)$(PATHSEP)avr-g++


#
# Include defconfig files:
#

-include .config_mach
-include .config_lang


#
# Add headers and source files and set some variables
#

# Arduino core
HARDWARE_DIR = $(ARDUINO_INSTALL_DIR)$(PATHSEP)hardware
HARDWARE_SRC = $(HARDWARE_DIR)$(PATHSEP)arduino$(PATHSEP)avr$(PATHSEP)cores$(PATHSEP)arduino

VPATHTEMP += $(HARDWARE_SRC)
CXXSRC = main.cpp

CSRC = wiring.c wiring_analog.c wiring_digital.c wiring_pulse.c wiring_shift.c \
      WInterrupts.c hooks.c
CXXSRC += WMath.cpp WString.cpp Print.cpp Tone.cpp abi.cpp new.cpp

# Machine setup
VPATHTEMP += language
ifeq ($(MACHINE_MODEL), Witbox)
	VPATHTEMP += config/witbox
else ifeq ($(MACHINE_MODEL), Hephestos)
	VPATHTEMP += config/hephestos
else ifeq ($(MACHINE_MODEL), Hephestos XL)
	VPATHTEMP += config/hephestos_xl
else ifeq ($(MACHINE_MODEL), Hephestos ZUM)
	VPATHTEMP += config/hephestos_zum
else ifeq ($(MACHINE_MODEL), Hephestos ZUM XL)
	VPATHTEMP += config/hephestos_zum_xl
else ifeq ($(MACHINE_MODEL), Hephestos ZUM Heated Bed)
	VPATHTEMP += config/hephestos_zum_heated_bed
else ifeq ($(MACHINE_MODEL), Witbox 2)
	VPATHTEMP += config/witbox_2
else ifeq ($(MACHINE_MODEL), Hephestos 2)
	VPATHTEMP += config/hephestos_2
endif

# Microcontroller setup
ifeq ($(HARDWARE_MOTHERBOARD), RAMPS)
MCU              ?= atmega2560
F_CPU            ?= 16000000
else ifeq  ($(HARDWARE_MOTHERBOARD), BQ_CNC)
MCU              ?= atmega2560
F_CPU            ?= 16000000
else ifeq ($(HARDWARE_MOTHERBOARD), BQ_ZUM_MEGA_3D)
MCU              ?= atmega2560
F_CPU            ?= 16000000
endif

# ATmega2560
VPATHTEMP += $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/variants/mega

# SPI library
VPATHTEMP += $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/libraries/SPI
CXXSRC += SPI.cpp

VPATHTEMP += .

# Display setup
ifeq ($(HARDWARE_DISPLAY), Graphic)
	VPATHTEMP += libraries/U8glib
	VPATHTEMP += libraries/U8glib/utility
	CSRC += u8g_com_arduino_common.c u8g_com_arduino_st7920_spi.c u8g_dev_st7920_128x64.c \
		u8g_com_api.c u8g_com_null.c u8g_ll_api.c u8g_pb.c u8g_pb32h1.c u8g_pb8h1.c u8g_page.c \
		u8g_bitmap.c u8g_clip.c u8g_delay.c u8g_font.c u8g_font_data.c u8g_state.c u8g_rect.c u8g_line.c
	CXXSRC += U8glib.cpp
	CXXSRC += ultralcd_st7920_u8glib_rrd.cpp
else ifeq ($(HARDWARE_DISPLAY), Character)
	VPATHTEMP += $(ARDUINO_INSTALL_DIR)/libraries/LiquidCrystal/src
	CXXSRC += LiquidCrystal.cpp 
	CXXSRC += ultralcd.cpp
endif

# SD library
VPATHTEMP += libraries/SdFat
CXXSRC += Sd2Card.cpp SdBaseFile.cpp SdFatUtil.cpp SdFile.cpp SdVolume.cpp

# Generation specific files
ifeq ($(HARDWARE_DISPLAY), Graphic)
	CXXSRC += GuiManager.cpp \
		StepperClass.cpp Language.cpp
	CXXSRC += LightManager.cpp PrintManager.cpp SDManager.cpp SerialManager.cpp SteppersManager.cpp FanManager.cpp \
		TemperatureControl.cpp StatsManager.cpp HeatedbedManager.cpp
	include ui/ui.mk
	VPATHTEMP += ui
endif

# Common files
CXXSRC += HelpersC++.cpp 

CXXSRC += motion_control.cpp planner.cpp stepper.cpp temperature.cpp cardreader.cpp \
		watchdog.cpp digipot_mcp4451.cpp vector_3.cpp qr_solve.cpp ConfigurationStore.cpp

CXXSRC += Action.cpp GuiAction.cpp AutoLevelManager.cpp OffsetManager.cpp StorageManager.cpp TemperatureManager.cpp

CXXSRC += Marlin_main.cpp MarlinSerial.cpp SDCache.cpp

# Define all object files.
OBJ = ${patsubst %.c,$(BUILD_DIR)/%.o,${CSRC}}
OBJ += ${patsubst %.cpp,$(BUILD_DIR)/%.o,${CXXSRC}}
OBJ += ${patsubst %.S,$(BUILD_DIR)/%.o,${ASRC}}

ifeq ($(OS_TYPE), WIN)
	OBJ_CORRECT = $(subst /,\,$(OBJ))
else
	OBJ_CORRECT = $(OBJ)
endif

# Add all the source directories as include directories too

# Format vpath in according to system
ifeq ($(OS_TYPE), WIN)
	VPATH_TMP = $(subst \\,\,${VPATHTEMP})
	VPATH_TMP2 = $(subst /,\,${VPATH_TMP})
	#VPATH_TMP3 = ${addsuffix ",${VPATH_TMP2}}
	#VPATH_TMP4 = ${addprefix ",${VPATH_TMP3}}
	#VPATH_TMP5 = $(subst $(space),;,${VPATH_TMP2})
	VPATH = $(subst ___,$(space),${VPATH_TMP2})
else
	VPATH = ${VPATHTEMP}
endif

CINCS = ${addprefix -I ,${VPATH}}
CXXINCS = ${addprefix -I ,${VPATH}}


# Tools Configuration:
# avrdude configuration:
AVRDUDE_PORT = $(UPLOAD_PORT)
AVRDUDE_WRITE_FLASH = -D -U flash:w:$(BUILD_DIR)/$(TARGET).hex:i
AVRDUDE_CONF = $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/etc/avrdude.conf
AVRDUDE_FLAGS = -v -C $(AVRDUDE_CONF) -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b $(UPLOAD_RATE)

# Compile tools configuration:
# C compiler:
CDEBUG = -g
CDEFS    = -DARDUINO=$(ARDUINO_VERSION) -DF_CPU=$(F_CPU) -DLANGUAGE=$(LANGUAGE) ${addprefix -D , $(DEFINES)}
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
#CSTANDARD =
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -w -ffunction-sections -fdata-sections -fno-use-cxa-atexit
CWARN = -Wall -Wstrict-prototypes

# C compiler flags:
CFLAGS := $(CDEFS) $(CDEBUG) $(CINCS) -O$(OPT) $(CWARN) $(CEXTRA) $(CTUNING)

# C++ compiler:
CXXDEFS  = $(CDEFS)

# C++ Compiler flags.
CXXFLAGS :=         $(CDEFS) $(CINCS) -O$(OPT) -Wall -std=c++11 $(CEXTRA) $(CTUNING)

# Assembly compiler:
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs

# Linker flags:
LDFLAGS = -lm

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -x assembler-with-cpp $(ASFLAGS)


#
# Make targerts
#

# Default target.
all: sizeafter

build: $(BUILD_DIR) elf hex defconfig

# Creates the object directory
$(BUILD_DIR):
	$(L) $(MAKE_DIR) $(BUILD_DIR)

elf: $(BUILD_DIR)/$(TARGET).elf
hex: $(BUILD_DIR)/$(TARGET).hex
eep: $(BUILD_DIR)/$(TARGET).eep
lss: $(BUILD_DIR)/$(TARGET).lss
sym: $(BUILD_DIR)/$(TARGET).sym

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(BUILD_DIR)/$(TARGET).hex
ELFSIZE = $(SIZE) --mcu=$(MCU) -C $(BUILD_DIR)$(PATHSEP)$(TARGET).elf; \
          $(SIZE)  $(BUILD_DIR)$(PATHSEP)$(TARGET).elf
sizebefore:
ifeq ($(OS_TYPE), UNIX)
	$(L) if [ -f $(BUILD_DIR)$(PATHSEP)$(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi
endif

sizeafter: build
ifeq ($(OS_TYPE), UNIX)
	$(L) if [ -f $(BUILD_DIR)/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi
endif

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000


coff: $(BUILD_DIR)/$(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(BUILD_DIR)$(PATHSEP)$(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(BUILD_DIR)$(PATHSEP)$(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym
.PRECIOUS: .o

.elf.hex:
	$(L) $(ECHO) "  COPY  $@"
	$(L) $(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

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
$(BUILD_DIR)/$(TARGET).elf: $(OBJ) Configuration.h
	$(L) $(ECHO) "  CXX   $@"
	$(L) $(CC) $(ALL_CXXFLAGS) -Wl,--gc-sections -o $@ -L. $(OBJ_CORRECT) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c Configuration.h Configuration_adv.h $(MAKEFILE)
	$(L) $(ECHO) "  CC    $<"
	$(L) $(CC) -MMD -c $(ALL_CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Configuration.h Configuration_adv.h $(MAKEFILE)
	$(L) $(ECHO) "  CXX   $<"
	$(L) $(CXX) -MMD -c $(ALL_CXXFLAGS) $< -o $@

# Flash the device
upload: $(BUILD_DIR)/$(TARGET).elf
ifeq (${AVRDUDE_PROGRAMMER}, arduino)
	stty hup < $(UPLOAD_PORT); true
endif
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)
ifeq (${AVRDUDE_PROGRAMMER}, arduino)
	stty -hup < $(UPLOAD_PORT); true
endif

# Clean the output files generated.
clean:
	$(L) $(ECHO) "  RMDIR $(BUILD_DIR)/"
	$(L) $(REMOVE_DIR) $(BUILD_DIR)

# Clean the output generated files and the board configuration.
cleanall: clean
	$(L) $(ECHO) "  RM    .config_mach"
	$(L) $(REMOVE) .config_mach
	$(L) $(ECHO) "  RM    .config_lang"
	$(L) $(REMOVE) .config_lang

### Configure machine targets
# 1st Generation printers
witbox:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)witbox$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

hephestos:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)hephestos$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

hephestos_xl:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)hephestos_xl$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

# 1st Generation printers with new board
hephestos_zum:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)hephestos_zum$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

hephestos_zum_xl:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)hephestos_zum_xl$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

hephestos_zum_heated_bed:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)hephestos_zum_heated_bed$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

# 2nd Generation printers
witbox_2:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)witbox_2$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

hephestos_2:
	$(L) $(ECHO) Configurating for $@ printer...
	$(L) $(ECHO) Language selected: English [EN] \(by default\)
	$(L) $(COPY) config$(PATHSEP)hephestos_2$(PATHSEP)Makefile .config_mach
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang


### Configure language targets
DE:
	$(L) $(ECHO) Changing language to: German [$@]
	$(L) $(COPY) language$(PATHSEP)de$(PATHSEP)Makefile .config_lang

EN:
	$(L) $(ECHO) Changing language to: English [$@]
	$(L) $(COPY) language$(PATHSEP)en$(PATHSEP)Makefile .config_lang

ES:
	$(L) $(ECHO) Changing language to: Spanish [$@]
	$(L) $(COPY) language$(PATHSEP)es$(PATHSEP)Makefile .config_lang

FI:
	$(L) $(ECHO) Changing language to: Finnish [$@]
	$(L) $(COPY) language$(PATHSEP)fi$(PATHSEP)Makefile .config_lang

FR:
	$(L) $(ECHO) Changing language to: French [$@]
	$(L) $(COPY) language$(PATHSEP)fr$(PATHSEP)Makefile .config_lang

IT:
	$(L) $(ECHO) Changing language to: Italian [$@]
	$(L) $(COPY) language$(PATHSEP)it$(PATHSEP)Makefile .config_lang

NL:
	$(L) $(ECHO) Changing language to: Dutch [$@]
	@ $(COPY) language$(PATHSEP)nl$(PATHSEP)Makefile .config_lang

PL:
	$(L) $(ECHO) Changing language to: Polish [$@]
	$(L) $(COPY) language$(PATHSEP)pl$(PATHSEP)Makefile .config_lang

PT:
	$(L) $(ECHO) Changing language to: Portuguese [$@]
	$(L) $(COPY) language$(PATHSEP)pt$(PATHSEP)Makefile .config_lang

defconfig: .config_mach .config_lang

verbose:
	$(L) $(ECHO) ARDUINO_INSTALL_DIR:
	$(L) $(ECHO) $(ARDUINO_INSTALL_DIR)		
	$(L) $(ECHO) CSRC:
	$(L) $(ECHO) $(CSRC)
	$(L) $(ECHO) CXXSRC:
	$(L) $(ECHO) $(CXXSRC)
	$(L) $(ECHO) OBJ:
	$(L) $(ECHO) $(OBJ)
	$(L) $(ECHO) OBJ_CORRECT:
	$(L) $(ECHO) $(OBJ_CORRECT)
	$(L) $(ECHO) VPATHTEMP:
	$(L) $(ECHO) $(VPATHTEMP)
	$(L) $(ECHO) VPATH:
	$(L) $(ECHO) $(VPATH)


# Define .PHONY targets
.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend sizebefore sizeafter upload verbose

ifeq ($(OS_TYPE), WIN)
.PHONY: DE EN ES FI FR IT NL PL PT
endif

# Automaticaly include the dependency files created by gcc
-include ${wildcard $(BUILD_DIR)/*.d}


