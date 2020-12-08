# This repository is maintained as an updated merge of Marlin with specific MKS fork's commits.
In order to do that, I'm maintaining a patchset above Marlin's firmware from [here](https://github.com/MarlinFirmware/Marlin) and the [MKS fork](https://github.com/makerbase-mks/Mks-Robin-Nano-Marlin2.0-Firmware).

This means that I'm not keeping a stable history (the bigChange branch is rebased regularly on top of Marlin). If you are not used to this kind of process, it means that you'll need to call `git fetch` followed by `git reset --hard origin/bigChange` (if you have local modification, you should `git stash` them beforehand)

## Features received from MKS 
There are some "bug fixes" in MKS concerning WIFI that are supposed to be merged here.
Makerbase integrated their most interesting feature in Marlin in August 2020 (mainly their nice GUI) so this is alread in Marlin.

![](https://github.com/makerbase-mks/Mks-Robin-Nano-Marlin2.0-Firmware/blob/master/Images/MKS_Robin_Nano_printing.png)

## My own code
Since I'm testing the code on my printer, I need to use some `Configuration.h` and `Configuration_adv.h`. However, this will likely be different for you, so I've started a branch called `Sapphire` that's supposedly supporting TwoTrees's CoreXY 3D printer. This branch is exactly equal to this branch but with an additional commit on top for the Sapphire Plus configuration file and the built firmware in the `Firmware/SapphirePlus` folder. Please notice that I've upgraded my printer with many mods, so it might or might now work for you (you'll probably need to re-tune the PID for the hot bed and the hot end). A description of the modification are provided [here](https://blog.cyril.by).


## Known bugs

The following bugs were found by running the firmware:
| Bug | In MKS version | Expected solution |
|-----|----------------|-------------------|
| Pause and resume does not work as expected | Yes | Need to implement 2 types of pauses: one for filament break detection (like the current), where resuming implies unloading and reloading the filament, the other for temporary pause and resume without filament/extruder operation (maybe a small retract?) |
| Crash in the file listing if the GCode contains a picture | Yes | Not found yet |
| No position report in the "move" menu, like before | Yes | Need to add the missing label and update it on position change |


## Build
As the firmware is based on Marlin2.0.x which is built on the core of PlatformIO, the buid compiling steps are the same as Marlin2.0.x. You can directly using [PlatformIO Shell Commands](https://docs.platformio.org/en/latest/core/installation.html#piocore-install-shell-commands), or using IDEs contain built-in PlatformIO Core(CLI), for example, [VSCode](https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode) and [Atom](https://docs.platformio.org/en/latest/integration/ide/atom.html). VSCode is recommended.

## About the gcode file preview
The images should be added to gcode file when slicing, and MKS has developed the [plugin for Cura](https://github.com/makerbase-mks/mks-wifi-plugin) to make it.

## About the image conversion
- Open [LVGL online image converter tool](https://lvgl.io/tools/imageconverter). 
- Open bmp images.
- Enter the saved file name.
- Choose color format:True color.
- Choose file output format:Binary RGB565.
- Start convertion.
- Save bin file.
- Copy the converted bin file to the assets folder.
- Copy the assets folder to the SD card.
- SD card is connected to the motherboard, and you can see the update interface after powering on.

## Firmware Can be run on Robin Nano V1.x and V2.x boards
## MKS Robin Nano V1.x build and update firmware

1. Build config:
     
- platformio.ini: 
     
     default_envs = mks_robin_nano35    
- Configuation.h: 
     
     #define MOTHERBOARD BOARD_MKS_ROBIN_NANO   
     
     #define TFT_LVGL_UI_FSMC
     
     //#define TFT_LVGL_UI_SPI

2. Update firmware:
   
- Enter the `.pio\build\mks_robin_nano35` directory, copy the `assets` folder and `Robin_nano35.bin` to the sd card
- SD card is connected to the motherboard, and you can see the update interface after powering on.   

## MKS Robin Nano V2.x build and update firmware

1. Build config:
     
- platformio.ini: 
     
     default_envs = mks_robin_nano35    
- Configuation.h: 
     
     #define MOTHERBOARD BOARD_MKS_ROBIN_NANO_V2   
     
     //#define TFT_LVGL_UI_FSMC
     
     #define TFT_LVGL_UI_SPI

2. Update firmware:
   
- Enter the `.pio\build\mks_robin_nano35` directory, copy the `assets` folder and `Robin_nano35.bin` to the sd card
- SD card is connected to the motherboard, and you can see the update interface after powering on.   

## More information about the Robin Nano V1.X
Please refer to [MKS Robin Nano github](https://github.com/makerbase-mks/MKS-Robin-Nano-V1.X).

##  More information about the Robin Nano V2.X
Please refer to [MKS-Robin-Nano-V2 wiki](https://github.com/makerbase-mks/MKS-Robin-Nano-V2/wiki/Marlin_firmware).

# Marlin 3D Printer Firmware

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
