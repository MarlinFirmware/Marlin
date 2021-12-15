# Modifications to PowerWASP firmware with a few extras
The later versions of the PowerWASP controller board have power loss detection on pin 65 (low on loss). this has been configured in POWER_LOSS_RECOVERY. Because of this there is no need to save each command line just in case of a power loss to the machine. The board will hold enough power to allow the print to be saved to the SD card.

The retract position on finishing the print has been set at Z190 X0 Y0

There are provisions made here to use ESP3D wireless printer control.

https://github.com/luc-github/ESP3D

The original PowerWASP board has available a internal serial port connection with Arduino Serial3.
The connections are as follows:
```
.------------.---------------------.
|            |  Serial3            |
|            |  [O] 5V             | Front
|    FAN     |  [O] GND            | panel
|            |  [O] TX             | side
|            |  [O] RX             |
`------------´---------------------´
```

With the option of wireless file upload to the SD card, the idea of updating the Firmware
from the SD can also be useful. This option has been selected, but requires a modified boot loader
that can be found here:

https://github.com/rainerumrobotics/MicroBridge-Arduino-ATMega2560

USB and SDCARD Firmware flashing for the ARDUINO ATMEGA 2560 and ADK
---------------------------------------------------------------------
The bootloader looks for byte in eeprom at address 0x3FF:

  -if it is set to 0xF0 bootloader will look for a bin file on the sdcard named
   firmware.bin and use it to flash the firmware then reset the byte to 0xFF so it
   does this only once

  -otherwise no action is taken and bootloader works as a arduino bootloader except
   some of the debugging functions are missing

Setup

  ADAFruit micro sdcard 5V ready:

  SDCARD on ATMEGA 2560 or ADK

  pin 50  -  DO

  pin 51  -  DI

  pin 52  -  CLK

  pin 53  -  CS

Generate BIN file for firmware update:

To generate a bin file you need issue the following command on you apps elf executable:

  avr-objcopy -I elf32-avr -O binary firmware.cpp.elf firmware.bin

this generates firmware.bin which can be put on the sd for flashing your firmware. You can find the elf
file in you apps build directory.

Using olimex usb to upload bootloader:

  avrdude -c stk500v2 -p m2560 -P /dev/tty.usbmodemfd131 -B 500 -e -u -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xD8:m -U efuse:w:0xFF:m -F

  avrdude -p m2560 -c stk500v2 -P /dev/tty.usbmodemfa131 -F -U flash:w:stk500boot.hex -b 115200 -B1 -U lock:w:0x0F:m

  avrdude -p m168 -c usbtiny -e -u -U lock:w:0x3f:m -U efuse:w:0x00:m -U hfuse:w:0xDD:m -U lfuse:w:0xFF:m


  avrdude -p m2560 -c stk500v2 -P /dev/tty.usbmodemfd131 -U flash:w:stk500boot_v2_mega2560.hex -U lock:w:0x0F:m -v
  avrdude -c stk500v2 -p m2560 -P /dev/tty.usbmodemfd131 -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xD8:m -U lfuse:w:0xFF:m -e -v
