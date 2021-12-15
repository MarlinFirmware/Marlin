# Ender-6 Configurations for Marlin Firmware

## First Connect a USB Cable

The Ender-6 doesn't have an accessible USB port, so it's a good idea to connect a USB cable to the micro-USB port on the motherboard. Even if the factory LCD is reflashed with compatible LCD firmware, not all features will be accessible. A USB cable allows the printer to be controlled from a host and will provide complete functionality.

## BLTouch Instructions

To use an installed BLTouch probe, enable the custom option `ENABLE_BLTOUCH_PROBE` at the top of `Configuration.h` to get all the settings needed to support the probe. Creality isn't shipping printers with a BLTouch option at this time, but the existence of factory mounts, wiring, and OEM firmware supporting it suggests this may change in the future.

NOTE: Due to slight manufacturing differences, you will still need to tune your probe's `NOZZLE_TO_PROBE_OFFSET` values, at least for Z. For best results use the Probe Offset Wizard.

## Flashing Instructions

NOTE: The bootloader on this printer remembers the name of the most recent firmware file that was flashed from SD, so you must give the firmware file a unique name or it will be ignored.

Copy the compiled "`firmware.bin`" file to an SD card, giving it a distinct name. Turn off the printer, insert the SD card, and turn the printer on. The update will take a few seconds.

NOTE: The factory LCD firmware is only compatible with Creality's firmware. The LCD will not show the progress bar increment and the standard UI will not function. This is expected.

## Marlin DGUS UI Instructions

The Ender-6 stock LCD can optionally use a [customized Marlin DGUS interface](https://github.com/coldtobi/Marlin_DGUS_Resources). With the stock LCD firmware Marlin can only be controlled from a host over USB (see first section).

- Copy the `DWIN_SET` folder to a microSD card. The microSD card must be smaller than 8 GB and formatted with a 4K cluster size.
- Power off the printer and disassemble the front panel to get access to the LCD board.
- Insert the SD card into the slot on the back of the LCD (not the main SD slot).
- Power on the printer. The screen will turn blue and display several messages, finishing with "SD Card Process... END!".
- Remove the microSD card and power cycle the printer.
- Once it looks good reassemble the front panel. You may need to physically rotate the LCD by 180˚ for proper UI orientation.

## Restoring Factory Firmware

The Creality firmware can be restored by downloading the [Ender-6 V1.0.1 firmware](https://www.creality.com/download) and following the flashing procedure above with the Creality-provided `DWIN_SET` folder.
