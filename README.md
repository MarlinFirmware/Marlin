# Marlin 3D Printer Firmware for Monoprice Delta Mini

A fork of Marlin firmware (2.5.2) for the Monoprice MP Mini Delta 3d printer

<img alt="Monoprice Mini Delta" height="240" align="right"
 src="https://github.com/papabricole/Marlin/raw/malyan-m300/mpminidelta.png" />

## WARNINGS

You need to replace the 12V 5A power brick by a 12V 10A one. Otherwise the printer will be very instable and randomly crash.

## How to build

 - Fork this repo
 - Create a new branch malyan-m300-YourName based onto malyan-m300
 - Edit Marlin/Configuration.h and make your changes
 - Make a pull request
 - Go to this repro, click on the 'Actions' tab, identify your branch
 - When the Build Actions is finished, download the 'firmware.zip' artifact
 
## How to flash

M300 printers require flashing via SD card. Use the SD card that came with the printer if possible. The bootloader is very picky about SD cards.

 - Copy fcupdate.flg and firmware.bin to your SD card
 - Insert the SD card into your printer
 - Power-cycle the printer. The first flash may take longer. Don't be surprised if the .99 version number doesn't show up until after the UI has launched the default screen.
 - Remove the SD card and delete the files from the card to prevent an accidental re-flash.

