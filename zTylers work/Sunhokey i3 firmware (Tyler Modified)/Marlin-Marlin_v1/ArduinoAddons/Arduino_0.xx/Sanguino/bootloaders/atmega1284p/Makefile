# Makefile for ATmegaBOOT
# E.Lins, 18.7.2005
# $Id$
#
# Instructions
#
# To make bootloader .hex file:
# make diecimila
# make lilypad
# make ng
# etc...
#
# To burn bootloader .hex file:
# make diecimila_isp
# make lilypad_isp
# make ng_isp
# etc...

# program name should not be changed...
PROGRAM    = optiboot

# The default behavior is to build using tools that are in the users
# current path variables, but we can also build using an installed
# Arduino user IDE setup, or the Arduino source tree.
# Uncomment this next lines to build within the arduino environment,
# using the arduino-included avrgcc toolset (mac and pc)
# ENV ?= arduino
# ENV ?= arduinodev
# OS ?= macosx
# OS ?= windows


# enter the parameters for the avrdude isp tool
ISPTOOL    = stk500v2
ISPPORT    = usb
ISPSPEED   = -b 115200

MCU_TARGET = atmega168
LDSECTIONS  = -Wl,--section-start=.text=0x3e00 -Wl,--section-start=.version=0x3ffe

# Build environments
# Start of some ugly makefile-isms to allow optiboot to be built
# in several different environments.  See the README.TXT file for
# details.

# default
fixpath = $(1)

ifeq ($(ENV), arduino)
# For Arduino, we assume that we're connected to the optiboot directory
# included with the arduino distribution, which means that the full set
# of avr-tools are "right up there" in standard places.
TOOLROOT = ../../../tools
GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

ifeq ($(OS), windows)
# On windows, SOME of the tool paths will need to have backslashes instead
# of forward slashes (because they use windows cmd.exe for execution instead
# of a unix/mingw shell?)  We also have to ensure that a consistent shell
# is used even if a unix shell is installed (ie as part of WINAVR)
fixpath = $(subst /,\,$1)
SHELL = cmd.exe
endif

else ifeq ($(ENV), arduinodev)
# Arduino IDE source code environment.  Use the unpacked compilers created
# by the build (you'll need to do "ant build" first.)
ifeq ($(OS), macosx)
TOOLROOT = ../../../../build/macosx/work/Arduino.app/Contents/Resources/Java/hardware/tools
endif
ifeq ($(OS), windows)
TOOLROOT = ../../../../build/windows/work/hardware/tools
endif

GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

else
GCCROOT =
AVRDUDE_CONF =
endif
#
# End of build environment code.


# the efuse should really be 0xf8; since, however, only the lower
# three bits of that byte are used on the atmega168, avrdude gets
# confused if you specify 1's for the higher bits, see:
# http://tinker.it/now/2007/02/24/the-tale-of-avrdude-atmega168-and-extended-bits-fuses/
#
# similarly, the lock bits should be 0xff instead of 0x3f (to
# unlock the bootloader section) and 0xcf instead of 0x2f (to
# lock it), but since the high two bits of the lock byte are
# unused, avrdude would get confused.

ISPFUSES    = $(GCCROOT)avrdude $(AVRDUDE_CONF) -c $(ISPTOOL) \
              -p $(MCU_TARGET) -P $(ISPPORT) $(ISPSPEED) \
              -e -u -U lock:w:0x3f:m -U efuse:w:0x$(EFUSE):m \
              -U hfuse:w:0x$(HFUSE):m -U lfuse:w:0x$(LFUSE):m
ISPFLASH    = $(GCCROOT)avrdude $(AVRDUDE_CONF) -c $(ISPTOOL) \
              -p $(MCU_TARGET) -P $(ISPPORT) $(ISPSPEED) \
              -U flash:w:$(PROGRAM)_$(TARGET).hex -U lock:w:0x2f:m

STK500 = "C:\Program Files\Atmel\AVR Tools\STK500\Stk500.exe"
STK500-1 = $(STK500) -e -d$(MCU_TARGET) -pf -vf -if$(PROGRAM)_$(TARGET).hex \
-lFF -LFF -f$(HFUSE)$(LFUSE) -EF8 -ms -q -cUSB -I200kHz -s -wt
STK500-2 = $(STK500) -d$(MCU_TARGET) -ms -q -lCF -LCF -cUSB -I200kHz -s -wt

OBJ        = $(PROGRAM).o
OPTIMIZE = -Os -fno-inline-small-functions -fno-split-wide-types -mshort-calls

DEFS       = 
LIBS       =

CC         = $(GCCROOT)avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(AVR_FREQ) $(DEFS)
override LDFLAGS       = $(LDSECTIONS) -Wl,--relax -Wl,--gc-sections -nostartfiles -nostdlib

OBJCOPY        = $(GCCROOT)avr-objcopy
OBJDUMP        = $(call fixpath,$(GCCROOT)avr-objdump)

SIZE           = $(GCCROOT)avr-size

# Test platforms
# Virtual boot block test
virboot328: TARGET = atmega328
virboot328: MCU_TARGET = atmega328p
virboot328: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200' '-DVIRTUAL_BOOT'
virboot328: AVR_FREQ = 16000000L
virboot328: LDSECTIONS  = -Wl,--section-start=.text=0x7e00 -Wl,--section-start=.version=0x7ffe
virboot328: $(PROGRAM)_atmega328.hex
virboot328: $(PROGRAM)_atmega328.lst

# 20MHz clocked platforms
#
# These are capable of 230400 baud, or 115200 baud on PC (Arduino Avrdude issue)
#

pro20: TARGET = pro_20mhz
pro20: MCU_TARGET = atmega168
pro20: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
pro20: AVR_FREQ = 20000000L
pro20: $(PROGRAM)_pro_20mhz.hex
pro20: $(PROGRAM)_pro_20mhz.lst

pro20_isp: pro20
pro20_isp: TARGET = pro_20mhz
# 2.7V brownout
pro20_isp: HFUSE = DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro20_isp: LFUSE = C6
# 512 byte boot
pro20_isp: EFUSE = 04
pro20_isp: isp

# 16MHz clocked platforms
#
# These are capable of 230400 baud, or 115200 baud on PC (Arduino Avrdude issue)
#

pro16: TARGET = pro_16MHz
pro16: MCU_TARGET = atmega168
pro16: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
pro16: AVR_FREQ = 16000000L
pro16: $(PROGRAM)_pro_16MHz.hex
pro16: $(PROGRAM)_pro_16MHz.lst

pro16_isp: pro16
pro16_isp: TARGET = pro_16MHz
# 2.7V brownout
pro16_isp: HFUSE = DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro16_isp: LFUSE = C6
# 512 byte boot
pro16_isp: EFUSE = 04
pro16_isp: isp

# Diecimila, Duemilanove with m168, and NG use identical bootloaders
# Call it "atmega168" for generality and clarity, keep "diecimila" for
# backward compatibility of makefile
#
atmega168: TARGET = atmega168
atmega168: MCU_TARGET = atmega168
atmega168: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
atmega168: AVR_FREQ = 16000000L 
atmega168: $(PROGRAM)_atmega168.hex
atmega168: $(PROGRAM)_atmega168.lst

atmega168_isp: atmega168
atmega168_isp: TARGET = atmega168
# 2.7V brownout
atmega168_isp: HFUSE = DD
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega168_isp: LFUSE = FF
# 512 byte boot
atmega168_isp: EFUSE = 04
atmega168_isp: isp

diecimila: TARGET = diecimila
diecimila: MCU_TARGET = atmega168
diecimila: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
diecimila: AVR_FREQ = 16000000L 
diecimila: $(PROGRAM)_diecimila.hex
diecimila: $(PROGRAM)_diecimila.lst

diecimila_isp: diecimila
diecimila_isp: TARGET = diecimila
# 2.7V brownout
diecimila_isp: HFUSE = DD
# Low power xtal (16MHz) 16KCK/14CK+65ms
diecimila_isp: LFUSE = FF
# 512 byte boot
diecimila_isp: EFUSE = 04
diecimila_isp: isp

atmega328: TARGET = atmega328
atmega328: MCU_TARGET = atmega328p
atmega328: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
atmega328: AVR_FREQ = 16000000L
atmega328: LDSECTIONS  = -Wl,--section-start=.text=0x7e00 -Wl,--section-start=.version=0x7ffe
atmega328: $(PROGRAM)_atmega328.hex
atmega328: $(PROGRAM)_atmega328.lst

atmega328_isp: atmega328
atmega328_isp: TARGET = atmega328
atmega328_isp: MCU_TARGET = atmega328p
# 512 byte boot, SPIEN
atmega328_isp: HFUSE = DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_isp: LFUSE = FF
# 2.7V brownout
atmega328_isp: EFUSE = 05
atmega328_isp: isp

atmega1284: TARGET = atmega1284p
atmega1284: MCU_TARGET = atmega1284p
atmega1284: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200' '-DBIGBOOT'
atmega1284: AVR_FREQ = 16000000L
atmega1284: LDSECTIONS  = -Wl,--section-start=.text=0x1fc00
atmega1284: $(PROGRAM)_atmega1284p.hex
atmega1284: $(PROGRAM)_atmega1284p.lst

atmega1284_isp: atmega1284
atmega1284_isp: TARGET = atmega1284p
atmega1284_isp: MCU_TARGET = atmega1284p
# 1024 byte boot
atmega1284_isp: HFUSE = DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega1284_isp: LFUSE = FF
# 2.7V brownout
atmega1284_isp: EFUSE = FD
atmega1284_isp: isp

# Sanguino has a minimum boot size of 1024 bytes, so enable extra functions
#
sanguino: TARGET = atmega644p
sanguino: MCU_TARGET = atmega644p
sanguino: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200' '-DBIGBOOT'
sanguino: AVR_FREQ = 16000000L
sanguino: LDSECTIONS  = -Wl,--section-start=.text=0xfc00
sanguino: $(PROGRAM)_atmega644p.hex
sanguino: $(PROGRAM)_atmega644p.lst

sanguino_isp: sanguino
sanguino_isp: TARGET = atmega644p
sanguino_isp: MCU_TARGET = atmega644p
# 1024 byte boot
sanguino_isp: HFUSE = DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
sanguino_isp: LFUSE = FF
# 2.7V brownout
sanguino_isp: EFUSE = 05
sanguino_isp: isp

# Mega has a minimum boot size of 1024 bytes, so enable extra functions
#mega: TARGET = atmega1280
mega: MCU_TARGET = atmega1280
mega: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200' '-DBIGBOOT'
mega: AVR_FREQ = 16000000L
mega: LDSECTIONS  = -Wl,--section-start=.text=0x1fc00
mega: $(PROGRAM)_atmega1280.hex
mega: $(PROGRAM)_atmega1280.lst

mega_isp: mega
mega_isp: TARGET = atmega1280
mega_isp: MCU_TARGET = atmega1280
# 1024 byte boot
mega_isp: HFUSE = DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
mega_isp: LFUSE = FF
# 2.7V brownout
mega_isp: EFUSE = 05
mega_isp: isp

# ATmega8
#
atmega8: TARGET = atmega8
atmega8: MCU_TARGET = atmega8
atmega8: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
atmega8: AVR_FREQ = 16000000L 
atmega8: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega8: $(PROGRAM)_atmega8.hex
atmega8: $(PROGRAM)_atmega8.lst

atmega8_isp: atmega8
atmega8_isp: TARGET = atmega8
atmega8_isp: MCU_TARGET = atmega8
# SPIEN, CKOPT, Bootsize=512B
atmega8_isp: HFUSE = CC
# 2.7V brownout, Low power xtal (16MHz) 16KCK/14CK+65ms
atmega8_isp: LFUSE = BF
atmega8_isp: isp

# ATmega88
#
atmega88: TARGET = atmega88
atmega88: MCU_TARGET = atmega88
atmega88: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
atmega88: AVR_FREQ = 16000000L 
atmega88: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega88: $(PROGRAM)_atmega88.hex
atmega88: $(PROGRAM)_atmega88.lst

atmega88_isp: atmega88
atmega88_isp: TARGET = atmega88
atmega88_isp: MCU_TARGET = atmega88
# 2.7V brownout
atmega88_isp: HFUSE = DD
# Low power xtal (16MHz) 16KCK/14CK+65ms
atemga88_isp: LFUSE = FF
# 512 byte boot
atmega88_isp: EFUSE = 04
atmega88_isp: isp


# 8MHz clocked platforms
#
# These are capable of 115200 baud
#

lilypad: TARGET = lilypad
lilypad: MCU_TARGET = atmega168
lilypad: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
lilypad: AVR_FREQ = 8000000L
lilypad: $(PROGRAM)_lilypad.hex
lilypad: $(PROGRAM)_lilypad.lst

lilypad_isp: lilypad
lilypad_isp: TARGET = lilypad
# 2.7V brownout
lilypad_isp: HFUSE = DD
# Internal 8MHz osc (8MHz) Slow rising power
lilypad_isp: LFUSE = E2
# 512 byte boot
lilypad_isp: EFUSE = 04
lilypad_isp: isp

lilypad_resonator: TARGET = lilypad_resonator
lilypad_resonator: MCU_TARGET = atmega168
lilypad_resonator: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
lilypad_resonator: AVR_FREQ = 8000000L
lilypad_resonator: $(PROGRAM)_lilypad_resonator.hex
lilypad_resonator: $(PROGRAM)_lilypad_resonator.lst

lilypad_resonator_isp: lilypad_resonator
lilypad_resonator_isp: TARGET = lilypad_resonator
# 2.7V brownout
lilypad_resonator_isp: HFUSE = DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
lilypad_resonator_isp: LFUSE = C6
# 512 byte boot
lilypad_resonator_isp: EFUSE = 04
lilypad_resonator_isp: isp

pro8: TARGET = pro_8MHz
pro8: MCU_TARGET = atmega168
pro8: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
pro8: AVR_FREQ = 8000000L
pro8: $(PROGRAM)_pro_8MHz.hex
pro8: $(PROGRAM)_pro_8MHz.lst

pro8_isp: pro8
pro8_isp: TARGET = pro_8MHz
# 2.7V brownout
pro8_isp: HFUSE = DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro8_isp: LFUSE = C6
# 512 byte boot
pro8_isp: EFUSE = 04
pro8_isp: isp

atmega328_pro8: TARGET = atmega328_pro_8MHz
atmega328_pro8: MCU_TARGET = atmega328p
atmega328_pro8: CFLAGS += '-DLED_START_FLASHES=3' '-DBAUD_RATE=115200'
atmega328_pro8: AVR_FREQ = 8000000L
atmega328_pro8: LDSECTIONS = -Wl,--section-start=.text=0x7e00 -Wl,--section-start=.version=0x7ffe
atmega328_pro8: $(PROGRAM)_atmega328_pro_8MHz.hex
atmega328_pro8: $(PROGRAM)_atmega328_pro_8MHz.lst

atmega328_pro8_isp: atmega328_pro8
atmega328_pro8_isp: TARGET = atmega328_pro_8MHz
atmega328_pro8_isp: MCU_TARGET = atmega328p
# 512 byte boot, SPIEN
atmega328_pro8_isp: HFUSE = DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_pro8_isp: LFUSE = FF
# 2.7V brownout
atmega328_pro8_isp: EFUSE = 05
atmega328_pro8_isp: isp

# 1MHz clocked platforms
#
# These are capable of 9600 baud
#

luminet: TARGET = luminet
luminet: MCU_TARGET = attiny84
luminet: CFLAGS += '-DLED_START_FLASHES=3' '-DSOFT_UART' '-DBAUD_RATE=9600'
luminet: CFLAGS += '-DVIRTUAL_BOOT_PARTITION'
luminet: AVR_FREQ = 1000000L
luminet: LDSECTIONS = -Wl,--section-start=.text=0x1d00 -Wl,--section-start=.version=0x1efe
luminet: $(PROGRAM)_luminet.hex
luminet: $(PROGRAM)_luminet.lst

luminet_isp: luminet
luminet_isp: TARGET = luminet
luminet_isp: MCU_TARGET = attiny84
# Brownout disabled
luminet_isp: HFUSE = DF
# 1MHz internal oscillator, slowly rising power
luminet_isp: LFUSE = 62
# Self-programming enable
luminet_isp: EFUSE = FE
luminet_isp: isp

#
# Generic build instructions
#
#

isp: $(TARGET)
        $(ISPFUSES)
        $(ISPFLASH)

isp-stk500: $(PROGRAM)_$(TARGET).hex
        $(STK500-1)
        $(STK500-2)

%.elf: $(OBJ)
        $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
        $(SIZE) $@

clean:
        rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex

%.lst: %.elf
        $(OBJDUMP) -h -S $< > $@

%.hex: %.elf
        $(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $< $@

%.srec: %.elf
        $(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O srec $< $@

%.bin: %.elf
        $(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O binary $< $@