# Ender 3 V2

## Flashing Firmware

The bootloader which handles flashing new firmware on this board remembers the last filename you used.

Therefore, to flash the compiled firmware binary onto the board you must give the "`firmware.bin`" file on the SD card a unique name, different from the name of the previous firmware file, or you will be greeted with a blank screen on the next boot.

# Updating the Display

To update the graphics and icons on the display:

- Copy the `DWIN_SET` folder to an SD card and insert the card into the slot on the back of the display unit.
- Power on the machine and wait for the screen to change from blue to orange.
- Power off the machine.
- Remove the SD card from the back of the display.
- Power on to confirm a successful flash.
