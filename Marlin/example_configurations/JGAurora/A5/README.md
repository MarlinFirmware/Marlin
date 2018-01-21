# Configuration for JGAurora A5 printer

## Firmware Installation Instructions

To install custom firmware:

- Download and install arduino IDE software from https://www.arduino.cc/en/Main/Software
- Plug in A5 to your computer via USB cable
- In arduino IDE, under the Tools menu, set:
  – **Board** to "Arduino/Genuino Mega or Mega 2560"
  – ***Processor*** to "Atmega 2560 (Mega 2560)"
  – **Port** to your 3D printer’s serial port. To figure out the correct serial port unplug the printer and observe the "Port" menu. Connect the printer's USB cable, re-open the "Port" menu and see if a new ports has appeared. This new port is likely to be your printer.
    - If you can’t find the right port or if your machine isn't detected, you may need to install a driver for the CH340 serial-to-USB interface chip in the printer.
      - [macOS CH340 Serial Driver](http://sampin.ch/ch340-driver-mac)
      - [Windows CH340 Serial Driver](https://sparks.gogo.co.nz/ch340.html)
- Install the u8glib library using the library manager in the menu "Sketch -> Include Library -> Manage Libraries."
- IMPORTANT: Unplug the 8-pin LCD panel header connector on your printer. The LCD should turn off completely. This is found underneath the LCD, and can be accessed from underneath the printer without removing any screws. Remember where it goes so you can put it back after flashing.
- Close all other programs (Cura, Repetier Host, JGCreat, etc.) that could connect to the serial port.
- Open the "Marlin.ino" file in Arduino IDE and wait for the new project window to appear.
- Select "Upload" from the "Sketch" menu or click the Upload button.
- Wait for the firmware to finish uploading and verifying. This may take a couple of minutes.
- Plug the LCD connector back in.
- In the Arduino IDE, select "Tools -> Serial monitor" to open up a simple console. In the popup menus, select "Newline" and a Baud Rate of "250000."
- In the input box, enter the following G-code commands:
  - `M502` (press return) to reset settings (in RAM) to your configured defaults.
  - `M500` (press return) to write settings and initialize the EEPROM.

## Graphical Display

The control panel included with the JGAurora interfaces only indirectly with Marlin, and this imposes some limitations. But you can use a RepRap Discount Full Graphic Smart Controller with the JGAurora A5 by applying the following additional settings in `Configuration.h`:

```cpp
#define ENCODER_PULSES_PER_STEP 5
#define ENCODER_STEPS_PER_MENU_ITEM 1
#define REVERSE_ENCODER_DIRECTION
#define REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
```

You may also be able to change `BEEPER_PIN` to use the piezo on the LCD controller instead of the one on the board.
