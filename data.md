# This file contains common pin mappings for the GTM32 PRO board in
# the Geeetech 301 printer. To use this config, the firmware should be
# compiled for the STM32F103 with "No bootloader", serial (on USART1
# PA10/PA9) communication and GPIO pins to set at micro-controller
# startup set to "!PB4,!PB5,!PB0,!PB1"

# The "make flash" command does not work on the Geeetech 301. Instead,
# after running "make", run the following command to flash the board:
#  stm32flash -w out/klipper.bin -v -i rts,-dtr,dtr -b 115200 /dev/ttyUSB0

# See docs/Config_Reference.md for a description of parameters.

Delta
STM32F103
1 extruder
Steps??
229 mm endstop
EPCOS 100K B57560G104F
LJ12A3
hd44780
