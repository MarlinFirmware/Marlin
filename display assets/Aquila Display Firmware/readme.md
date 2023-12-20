# Updating the Display

To update the graphics and icons on the display:

- [Direct Download Link](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.1.x/display%20assets/Aquila%20Display%20Firmware/Firmware%20Sets) to just the Firmware Sets folder.
- Copy one of the `DWIN_SET`s from the Firmware Sets folder to an SD card (Formatted to FAT32 4096 or 4k allocation size). Remove the identifier so the folder is just named `DWIN_SET`. Remove the back cover of the display and insert the card into the microSD slot on the back of the display unit's PCB.
- Power on the machine and wait for the screen to change from blue to orange.
- Power off the machine.
- Remove the SD card from the back of the display.
- Power on to confirm a successful flash.

# Revert to the original font
You can revert back to the original font simply by downloading the folder "DWIN Font (Original)" (renamed to DWIN_SET) to the SD card and flash to your display.

## Troubleshooting
If flashing is not working (e.g. the blue screen only flashes for a second before turning to a red/orange screen):
- Reformat the SD card in FAT32 with 4k/4096 allocation size
- Leave only the folder `DWIN_SET`
- Use a storage size SD card 8GB or less

## Credits
- [**Voxelab**](https://github.com/Voxelab-64) | [voxelab3dp.com/download](https://www.voxelab3dp.com/download)
- [**alexqzd**](https://github.com/alexqzd) for editing and making icons available
- [**ClassicRocker883**](https://github.com/classicrocker883) for updating the icons

## DWIN_SET CRC
|  Firmware Set        |File                        | SHA-256
|----------------------|----------------------------|-----------------------
|                      |T5UIC1.CFG                  | E1C573639BFA2B3A06C2FA7AD3CAB483653DD3DC383217FF653FAB3145458095
|                      |T5UIC1_V20_4??_191022.BIN   | F8F9A3075AE5516328044ACB79CA522753133B66F1ECBD108E7B5DB2F3FF2FE5
|                      |0T5UIC1.HZK                 | 27F3AE70117DC031E6EA542654CA03B89BB9A0592B23AA9B7E452C35583C0108
|Custom                |9.ICO                       | 6BF0CFF10D61DE64D98BF286B0B60EE91FA88B0CA7FC7AF777CB6C9C71F15F1C
|Gotcha                |9.ICO                       | D90B25EE7675E61FFA8E8CD1B4E9435DFD4A51FADC767D291B50064C005AA8B4
|Original              |9.ICO                       | B19C67B762B6A29DC1D7BE5E2A5EDCD58E31EC40BC28125A2F361986BE019000
|Voxelab Original Light|9.ICO                       | 9C6274182E39E2D1A2FEC8C907C10F9590AFD16ABEB4027B2ADB560A642FBB8A
|Voxelab Red           |9.ICO                       | E2FB515545AFF7AE8C4AD794CDAEFA4E1A8B5E9E84A3CAD6B04898F68ECDD5B5
