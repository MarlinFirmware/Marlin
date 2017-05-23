# Configuration for Fabrikator Mini
This example configuration is for a Turnigy Fabrikator Mini 1.5

It does work fine with stock printers without any modifications 
and should work with the following modifications:

* RepRapDiscount Full Graphic Smart Controller
* tinyFab Heated Bed MK2 (http://tinyfab.xyz)


# Instructions 

## Configuration

From this directory

    cp Configuration.h ../..
    cp Configuration_adv.h ../..

## Compile Firmware

  - Start the Arduino IDE.
  - Select Tools -> Board -> Arduino Mega 2560
  - Select the correct serial port in Tools -> Serial Port (usually /dev/ttyUSB0)
  - Open Marlin.pde or .ino
  - Click the Verify/Compile button

## Flash Firmware

  - Click the Upload button. If all goes well the firmware is uploading

# Notes

## Hotend fan not starting
These configs will only start the hotend fan if a hotend temperature of 50°C has been 
reached, so don't be irritated if your hotend fan doesn't start right away. It will 
also switch off once the hotend temperature drops below 50°C again.

## LCD display empty
If you plugged in the LCD controller and it does light up, but does not show any output,
you will have to rotate the plugs on the display controller board in their sockets so
that the red leads on the cable are pointing towards the SD card reader.
This may require you to cut/file off the rib from the plug, or pull the connector socket
off of the display controller board (which should leave the pins in place).

### MINTEMP error
If you don't have a heated bed installed, make sure that you disable the heated bed option
in your software or printer profile. If the controller tries to heat the bed without a
thermistor present, it will immediately halt the printer and ask you to reset it.
This doesn't cause any damage, especially if you don't have a heated bed anyways, but it
prevents your printer from melting down if the thermistor is broken - or Marlin not being
able to read it, because you don't have a heated bed installed.
