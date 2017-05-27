# Configuration for Fabrikator Mini
This example configuration is for a Turnigy Fabrikator Mini 1.5

It does work fine with stock printers without any modifications.

# Instructions 

## Configuration

From this directory

    $ cp *.h ../..

## Compile Firmware

  - Start the Arduino IDE.
  - Select Tools -> Board -> Arduino Mega 2560.
  - Open Marlin.pde or .ino
  - Click the Verify/Compile button.

## Flash Firmware

  - Make sure no other software (repetier-host etc) is talking on the printer port.
  - Select the correct serial port in Tools -> Serial Port (usually /dev/ttyUSB0).
  - Click the Upload button. If all goes well the firmware is uploading (blue LEDs flashing).

