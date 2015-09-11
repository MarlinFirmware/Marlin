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
ARDUINO_INSTALL_DIR  ?= ${ARDUINO}
ARDUINO_VERSION      ?= 164

# Directory used to build files in. It contains all the build files, from
# object files to the final hex file:
BUILD_DIR            ?= applet

# Programmer configuration:
AVRDUDE_PROGRAMMER ?= wiring
UPLOAD_RATE        ?= 115200
UPLOAD_PORT        ?= /dev/ttyUSB*

############################################################################
# Below here nothing should be changed.

# Makefile Setings
MAKEFILE = Makefile

# Output format
FORMAT = ihex

AVR_TOOLS_PATH ?= $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/bin/

VPATH = .
VPATH += $(BUILD_DIR)

-include .config

LANGUAGE ?= EN
UPPER_LANGUAGE = $(shell echo $(LANGUAGE) | tr a-z A-Z)

# Machine setup
ifeq ($(MACHINE_MODEL), Witbox)
	VPATH += config/witbox
else ifeq ($(MACHINE_MODEL), Hephestos)
	VPATH += config/hephestos
else ifeq ($(MACHINE_MODEL), Hephestos XL)
	VPATH += config/hephestos_xl
else ifeq ($(MACHINE_MODEL), Witbox 2)
	VPATH += config/witbox_2
else ifeq ($(MACHINE_MODEL), Hephestos 2)
	VPATH += config/hephestos_2
endif

#
# Microcontroller setup
#

# RAMPS
ifeq ($(HARDWARE_MOTHERBOARD), RAMPS)
MCU              ?= atmega2560
F_CPU            ?= 16000000

# bq CNC
else ifeq  ($(HARDWARE_MOTHERBOARD), BQ_CNC)
MCU              ?= atmega2560
F_CPU            ?= 16000000

# bq ZUM Mega 3D
else ifeq ($(HARDWARE_MOTHERBOARD), BQ_ZUM_MEGA)
MCU              ?= atmega2560
F_CPU            ?= 16000000

endif


HARDWARE_DIR = $(ARDUINO_INSTALL_DIR)/hardware
HARDWARE_SRC = $(HARDWARE_DIR)/arduino/avr/cores/arduino

TARGET = $(notdir $(CURDIR))

VPATH += $(HARDWARE_SRC)
CXXSRC = main.cpp

VPATH += $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/variants/mega
CSRC = wiring.c wiring_analog.c wiring_digital.c wiring_pulse.c wiring_shift.c \
      WInterrupts.c hooks.c
CXXSRC += WMath.cpp WString.cpp Print.cpp Tone.cpp abi.cpp new.cpp

VPATH += $(ARDUINO_INSTALL_DIR)/hardware/arduino/avr/libraries/SPI
CXXSRC += SPI.cpp


#
# Display setup
#

ifeq ($(HARDWARE_DISPLAY), Graphic)
	VPATH += libraries/U8glib
	VPATH += libraries/U8glib/utility
	CSRC += u8g_com_arduino_common.c u8g_com_arduino_st7920_spi.c u8g_dev_st7920_128x64.c \
		u8g_com_api.c u8g_com_null.c u8g_ll_api.c u8g_pb.c u8g_pb32h1.c u8g_pb8h1.c u8g_page.c \
		u8g_bitmap.c u8g_clip.c u8g_delay.c u8g_font.c u8g_font_data.c u8g_state.c u8g_rect.c u8g_line.c
	CXXSRC += U8glib.cpp
	CXXSRC += ultralcd_st7920_u8glib_rrd.cpp
else ifeq ($(HARDWARE_DISPLAY), Character)
	VPATH += $(ARDUINO_INSTALL_DIR)/libraries/LiquidCrystal/src
	CXXSRC += LiquidCrystal.cpp 
	CXXSRC += ultralcd.cpp
endif

VPATH += libraries/SdFat
CXXSRC += Sd2Card.cpp SdBaseFile.cpp SdFatUtil.cpp SdFile.cpp SdVolume.cpp

ifeq ($(HARDWARE_DISPLAY), Graphic)
	CXXSRC += GuiManager.cpp GuiPainter.cpp GuiImpl_witbox_2.cpp \
		Icon.cpp \
		Option.cpp OptionToggle.cpp OptionLaunch.cpp\
		Screen.cpp ScreenAbout.cpp ScreenComplete.cpp ScreenEmergency.cpp ScreenFile.cpp \
		ScreenInactivity.cpp ScreenLanguage.cpp ScreenList.cpp ScreenMenu.cpp ScreenPrint.cpp \
		ScreenSplash.cpp ScreenTransition.cpp ScreenSerial.cpp ScreenSetting.cpp ScreenSwitch.cpp \
		Stepper.cpp Language.cpp

	CXXSRC += LightManager.cpp PrintManager.cpp SDManager.cpp SteppersManager.cpp StorageManager.cpp \
		TemperatureManager.cpp ViewManager.cpp
endif

CXXSRC += HelpersC++.cpp 

CXXSRC += motion_control.cpp planner.cpp stepper.cpp temperature.cpp cardreader.cpp \
		watchdog.cpp digipot_mcp4451.cpp vector_3.cpp qr_solve.cpp ConfigurationStore.cpp

CXXSRC += Action.cpp GuiAction.cpp AutoLevelManager.cpp OffsetManager.cpp

CXXSRC += Marlin_main.cpp MarlinSerial.cpp

# Define all object files.
OBJ = ${patsubst %.c, $(BUILD_DIR)/%.o, ${CSRC}}
OBJ += ${patsubst %.cpp, $(BUILD_DIR)/%.o, ${CXXSRC}}
OBJ += ${patsubst %.S, $(BUILD_DIR)/%.o, ${ASRC}}

# Program settings
CC = $(AVR_TOOLS_PATH)avr-gcc
CXX = $(AVR_TOOLS_PATH)avr-g++

OBJCOPY = $(AVR_TOOLS_PATH)avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)avr-objdump
AR  = $(AVR_TOOLS_PATH)avr-ar
SIZE = $(AVR_TOOLS_PATH)avr-size
NM = $(AVR_TOOLS_PATH)avr-nm
AVRDUDE = $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/bin/avrdude

MOVE = mv -f
COPY = cp
REMOVE = rm -f

# Place -D or -U options here
CDEFS    = -DF_CPU=$(F_CPU) ${addprefix -D , $(DEFINES)}
CXXDEFS  = $(CDEFS)

# Add all the source directories as include directories too
CINCS = ${addprefix -I ,${VPATH}}
CXXINCS = ${addprefix -I ,${VPATH}}

# C Compiler flags.
#CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct \
	-fshort-enums -w -ffunction-sections -fdata-sections \
	-DARDUINO=$(ARDUINO_VERSION)

ifneq ($(HARDWARE_MOTHERBOARD),)
#CTUNING += -DMOTHERBOARD=${HARDWARE_MOTHERBOARD}
endif
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
CEXTRA = -fno-use-cxa-atexit

CFLAGS := $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CEXTRA) $(CTUNING)

# C++ Compiler flags.
CXXFLAGS :=         $(CDEFS) $(CINCS) -O$(OPT) -Wall -std=c++11 $(CEXTRA) $(CTUNING)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs

# Linker flags
LDFLAGS = -lm

# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(UPLOAD_PORT)
AVRDUDE_WRITE_FLASH = -D -U flash:w:$(BUILD_DIR)/$(TARGET).hex:i
ifeq ($(shell uname -s), Linux)
AVRDUDE_CONF = $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/etc/avrdude.conf
else
AVRDUDE_CONF = $(ARDUINO_INSTALL_DIR)/hardware/tools/avr/etc/avrdude.conf
endif
AVRDUDE_FLAGS = -v -C $(AVRDUDE_CONF) \
	-p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
	-b $(UPLOAD_RATE)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -x assembler-with-cpp $(ASFLAGS)

# set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif

# Custom functions
define check_language_1gen
	@case $(UPPER_LANGUAGE) in \
		DE) \
			echo "Language selected: German [DE]" ;; \
		EN) \
			echo "Language selected: English [EN]" ;; \
		ES) \
			echo "Language selected: Spanish [ES]" ;; \
		FI) \
			echo "Language selected: Finnish [FI]" ;; \
		FR) \
			echo "Language selected: French [FR]" ;; \
		IT) \
			echo "Language selected: Italian [IT]" ;; \
		PL) \
			echo "Language selected: Polish [PL]" ;; \
		PT) \
			echo "Language selected: Portuguese [PT]" ;; \
		*)  \
			echo "Invalid language: $(UPPER_LANGUAGE)" ; \
			echo "Language selected: English [EN] (default)" ;; \
	esac
endef

define check_language_2gen
	@case $(UPPER_LANGUAGE) in \
		DE) \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE]" ;; \
		EN) \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE]" ;; \
		ES) \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE]" ;; \
		FR) \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE]" ;; \
		IT) \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE]" ;; \
		PT) \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE]" ;; \
		SE) \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE]" ;; \
		*) \
			echo "Invalid language: $(UPPER_LANGUAGE)" ; \
			echo "Language selected: Multilanguage [DE EN ES FR IT PT SE] (default)" ;; \
	esac
endef

# Default target.
all: sizeafter

build: $(BUILD_DIR) elf hex .config

# Creates the object directory
$(BUILD_DIR):
	$P mkdir -p $(BUILD_DIR)

elf: $(BUILD_DIR)/$(TARGET).elf
hex: $(BUILD_DIR)/$(TARGET).hex
eep: $(BUILD_DIR)/$(TARGET).eep
lss: $(BUILD_DIR)/$(TARGET).lss
sym: $(BUILD_DIR)/$(TARGET).sym

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(BUILD_DIR)/$(TARGET).hex
ELFSIZE = $(SIZE) --mcu=$(MCU) -C $(BUILD_DIR)/$(TARGET).elf; \
          $(SIZE)  $(BUILD_DIR)/$(TARGET).elf
sizebefore:
	$P if [ -f $(BUILD_DIR)/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter: build
	$P if [ -f $(BUILD_DIR)/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi


# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000


coff: $(BUILD_DIR)/$(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(BUILD_DIR)/$(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(BUILD_DIR)/$(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym
.PRECIOUS: .o

.elf.hex:
	$(Pecho) "  COPY  $@"
	$P $(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

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
	$(Pecho) "  CXX   $@"
	$P $(CC) $(ALL_CXXFLAGS) -Wl,--gc-sections -o $@ -L. $(OBJ) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c Configuration.h Configuration_adv.h $(MAKEFILE)
	$(Pecho) "  CC    $<"
	$P $(CC) -MMD -c $(ALL_CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.cpp Configuration.h Configuration_adv.h $(MAKEFILE)
	$(Pecho) "  CXX   $<"
	$P $(CXX) -MMD -c $(ALL_CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Configuration.h Configuration_adv.h $(MAKEFILE)
	$(Pecho) "  CXX   $<"
	$P $(CXX) -MMD -c $(ALL_CXXFLAGS) $< -o $@


# Flash the device
upload: $(BUILD_DIR)/$(TARGET).elf
ifeq (${AVRDUDE_PROGRAMMER}, arduino)
	stty hup < $(UPLOAD_PORT); true
endif
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)
ifeq (${AVRDUDE_PROGRAMMER}, arduino)
	stty -hup < $(UPLOAD_PORT); true
endif

# Clean the output files generated
clean:
	$(Pecho) "  RM    $(BUILD_DIR)/*"
	$P $(REMOVE) $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).eep $(BUILD_DIR)/$(TARGET).cof $(BUILD_DIR)/$(TARGET).elf \
		$(BUILD_DIR)/$(TARGET).map $(BUILD_DIR)/$(TARGET).sym $(BUILD_DIR)/$(TARGET).lss $(BUILD_DIR)/$(TARGET).cpp \
		$(OBJ) $(LST) $(CSRC:.c=.s) $(CSRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)
	$(Pecho) "  RMDIR $(BUILD_DIR)/"
	$P rm -rf $(BUILD_DIR)

cleanall: clean
	$(Pecho) "  RM    .config"
	$P $(REMOVE) .config

# Configuration build enviroment targets
# 1st Generation printers
witbox:
	@echo Configurating for $@ printer...
	@echo Language: ${UPPER_LANGUAGE}
	@cp config/witbox/Makefile .config
	@echo >> .config
	@echo LANGUAGE ?= ${UPPER_LANGUAGE} >> .config
	$(call check_language_1gen)

hephestos:
	@echo Configurating for $@ printer...
	@echo Language: ${UPPER_LANGUAGE}
	@cp config/hephestos/Makefile .config
	@echo >> .config
	@echo LANGUAGE ?= ${UPPER_LANGUAGE} >> .config
	$(call check_language_1gen)

hephestos_xl:
	@cp config/hephestos_xl/Makefile .config
	@echo Language: ${UPPER_LANGUAGE}
	@echo Configurating for $@ printer...
	@cp config/hephestos_xl/Makefile .config
	@echo >> .config
	@echo LANGUAGE ?= ${UPPER_LANGUAGE} >> .config
	$(call check_language_1gen)

# 2nd Generation printers
witbox_2:
	@echo Configurating for $@ printer...
	@echo Language: ${UPPER_LANGUAGE}
	@cp config/witbox_2/Makefile .config
	@echo >> .config
	@echo LANGUAGE ?= ${UPPER_LANGUAGE} >> .config
	$(call check_language_2gen)

hephestos_2:
	@echo Configurating for $@ printer...
	@echo Language: ${UPPER_LANGUAGE}
	@cp config/hephestos_2/Makefile .config
	@echo >> .config
	@echo LANGUAGE ?= ${UPPER_LANGUAGE} >> .config
	$(call check_language_2gen)

# Define .PHONY targets
.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend sizebefore sizeafter upload

# Automaticaly include the dependency files created by gcc
-include ${wildcard $(BUILD_DIR)/*.d}
