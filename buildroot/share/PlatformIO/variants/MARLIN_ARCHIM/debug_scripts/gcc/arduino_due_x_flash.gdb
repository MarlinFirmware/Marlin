#*******************************************************
#
#  Connect to J-Link and debug application in flash on SAM3X.
#

# Define 'reset' command
define reset

# Connect to the J-Link gdb server
target remote localhost:2331

# Reset the chip to get to a known state
monitor reset

# Select flash device
monitor flash device = AT91SAM3X8E

# Enable flash download and flash breakpoints
monitor flash download = 1

# Load the program
load

# Reset peripheral (RSTC_CR)
set *0x400e1a00 = 0xA5000004

# Initialize PC and stack pointer
mon reg sp=(0x80000)
#set *0x80004 = *0x80004 & 0xFFFFFFFE
mon reg pc=(0x80004)

info reg

break main

# End of 'reset' command
end
