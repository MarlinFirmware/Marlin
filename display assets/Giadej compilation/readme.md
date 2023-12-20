# Giadej boot image and icons compilation

<img align="left" src="Boot_by_Giadej.jpg" height="400" />
<img  src="preview1.jpg"  height="400" />

## How to install

[**Click to Download DWIN_SET**](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.1.x/display%20assets/Giadej%20compilation/DWIN_SET) (for DWIN screen units)
[**Click to Download private**](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.1.x/display%20assets/Giadej%20compilation/private) (for DACAI screen units)

1. Get a Micro SD card of 8GB or less.
1. Format the SD card to FAT32 with a 4k/4096 sector allocation size
1. Copy the `DWIN_SET`(DWIN), `private`(DACAI) folder in the Root of SD
1. Turn off your printer
1. Disconnect and disassemble the screen unit
1. Install the SD card into the slot of the screen unit
1. Reconnect the screen to the printer
1. Turn on the printer and wait for the display to change color from blue to red/orange
1. Verify that the screen assets were updated
1. Turn off the printer and remove the SD card from the screen unit
1. Reassemble the screen unit

>If you want to change only the icons, then you can leave only the **9.ICO** file
inside of the DWIN_SET folder.

If you weren't able to update the display, verify the format of the SD Card
(MBR, FAT32 and allocation unit of 4096 bytes) and the CRC of the files.


<img src="../DWIN_SET-folder.jpg"  height="350" />

Be sure to leave **only** the `DWIN_SET` or `private`folder at the root of the card.


<img src="preview2.jpg"  height="400" />

## Credits
- [**Giadej**](https://github.com/Giadej) for Main and Control panel icons, and custom Boot-Screen
- [**Lighty1989**](https://github.com/Lighty1989) for "XYZ Move Axis" icons and "Bed Points" icons
- [**Gothcha**](https://github.com/gothcha) for other icons
- [**ClassicRocker883**](https://github.com/classicrocker883) for updating the icons

## DWIN_SET CRC
|File                        | SHA-256
|----------------------------|-----------------------
|9.ICO                       | 47939E140DE3E55F7BEDFBDC70068C228A8BDE0E72BE673635ED216311A6E5C5
|T5UIC1.CFG                  | E1C573639BFA2B3A06C2FA7AD3CAB483653DD3DC383217FF653FAB3145458095
|T5UIC1_V20_4??_191022.BIN   | F8F9A3075AE5516328044ACB79CA522753133B66F1ECBD108E7B5DB2F3FF2FE5
|0T5UIC1.HZK                 | 27F3AE70117DC031E6EA542654CA03B89BB9A0592B23AA9B7E452C35583C0108
