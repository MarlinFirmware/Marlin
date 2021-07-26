# Updating the Display

To update the graphics and icons on the display:

- Copy one of the `DWIN_SET`s from the Firmware Sets folder to an SD card. Remove the identifier so the folder is just named `DWIN_SET`. Remove the back cover of the display and insert the card into the microSD slot on the back of the display unit's PCB.
- Power on the machine and wait for the screen to change from blue to orange.
- Power off the machine.
- Remove the SD card from the back of the display.
- Power on to confirm a successful flash.

## Troubleshooting
If flashing is not working (e.g. the blue screen only flashes for a second before the orange screen):
- Reformat the SD card in FAT32 with 4k allocations
