# Firmware and information for BIQU 3D printers

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
[![Build Status](https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/MarlinFirmware/Marlin/actions)

<img align="right" width=180 src="https://yt3.ggpht.com/ytc/AAUvwniRECzqQDhtX2KDyOL3bc2NMap_PMMvdhDrHGQW3Q=s176-c-k-c0x00ffffff-no-rj" />

[//]: # (<img align="right" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />)

You likely have found your way here because you are looking for the latest firmware for your BIQU machine or because you are looking for some information on how to tune your machine/resolve an issue. This repository has been developed over time to include as much valuable information as possible. In order to get the most out of it please read the readme files in detail. This will save you time and help the author to help others who's issues are not covered in this material.

This repo currently contains firmware and information related to the following BIQU machines:

BIQU Machine Model | Latest Marlin Supported | Latest TFT Build
------------ | ------------- | -------------
BIQU BX | 2.0.6.1 | Not applicable
BIQU B1 | 2.0.7.2 | 25 December 2020

## Before you consider firmware

Many people experience issues which they believe can be solved by firmware but are actually mechanical in nature. In order to help people to solve such issues several "gists" have been compiled which guide you on how to set up the mechanics of your machine. While it is always recommended to run the latest firmware you should definitely take the time to read through these guides first as there are some issues which not even the most refined firmware will solve.

These guides should **definitely** be reviewed if you are assembling your machine for the first time but they should also be reviewed even if you have assembled your machine and just want to squeeze the most out of it. Following the steps in these guides will help you to avoid such things as:

1. Uneven first layers.
2. Nozzle hitting prints.
3. Z-banding
4. Many other print defects caused by poor mechanics.

BIQU Machine Model | Mechanical setup guide
------------ | -------------
BIQU BX | https://gist.github.com/looxonline/7f6a628debfa35c60a7dd3dc225d95bc
BIQU B1 | https://gist.github.com/looxonline/808a204f4f881462403f2baeefc1f69b

## Using the firmware from this repo.

People love to add on to their printers. For this reason there cannot be just a single Marlin firmware that satisfies all of the variations of BIQU printers out there. This repo aims to create firmware for the most popular printer variants and keep it up to date with the latest Marlin releases.

Source code for the FW is provided so that those with the ability can make additional modifications as they would like.

Each firmware variant is stored in a branch. You don't need to know much about github to know how to select the correct branch for your printer. It's really easy. Just select the name that best matches your printer variant from the drop down list and you will be on the correct branch.

The available branches for the B1 are:

Branch Name | Variant Properties
------------ | -------------
B1_STOCK | If you have built your B1 out of the box and done nothing else to it then this is the branch for you.
B1_ABL | If you have added an ABL sensor (BL Touch or other) then this is the branch for you. This does a 3x3 probe on the bed which is not the highest resolution but is often enough for beds that are not badly warped.
B1_ABL_HIGH_RES | If you have added an ABL sensor (BL Touch or other) and you want a higher res on the bed probing because of slightly more warping and are happy to pay a small time penalty at the start of each print then this is for you. This does a 4 x 4 probe.
B1_ABL_SUPER_RES | If you have added an ABL sensor (BL Touch or other) and you want an incredibly high res probe of the bed and are willing to wait a few minutes extra at the start of each print then this is for you.  This does a 5 x 5 probe.
B1_UBL_INSANE_RES | READ ME FIRST!!! This branch is strictly in beta. Before you even think about using it you must make sure that you are using this mount for your BL touch and not the stock one: https://www.thingiverse.com/thing:4564987. If you try to use it with the stock mount and the nozzle crashes into the bed then you can't say I didn't warn you. This branch has been designed to help people who have very warped beds (such as the B1 unit I have). Many printers in this range ship like this and many people will just use a glass bed to overcome it. However, I really enjoy the adhesion properties of the SSS bed and therefore wanted to be able to use it, hence this branch was born. The branch uses UBL instead of ABL which basically means that a highly detailed probe of 15 x 15 points is taken on a once off basis and then three points are sampled before each print just to check whether that mesh has changed orientation at all. The results on my bed were remarkable. For detailed usage instructions please follow the gist located here: https://gist.github.com/looxonline/eaa426a1be67148f1cf6242e0e8efbcb
B1_ABL_SFS | On hold due to lack of a meaningful userbase.



The available branches for the BX are listed below. Should you choose to use one of these branches then you **must** read through the gist located here as well as the "Using the firmware on the BX" section below https://gist.github.com/looxonline/94510f433c0f40ddcacbbecc384e844f.

Branch Name | Variant Properties
------------ | -------------
BX_ALMOST_STOCK | This is a unified version of firmware with many features (filament runout, power loss recovery) enabled but turned off in the firmware. If you have the needed hardware installed on your machine then you can just turn them on using g-codes. Using this firmware does require you to remove the z-endstop switch which is actually completely unnecessary when running an ABL probe and in fact more of a source of problems than anything else. It otherwise differs from the standard BIQU firmware in that it enables some useful features which have not been enabled in theirs such as automatic z-leveling and linear advance. It also increases the maximum acceleration speeds to levels which are more suited to such a capable printer. You can also apply the fan silencing modification (see the opening gist for the BX) using this firmware but you still have the option to run it without applying the modification. Make sure that you read the section below on how to use it before you install it.
BX_PCB | Want to use your BX to make printed circuit boards? This is the distro for you. Just don't try to use this firmware to print anything otherwise you're not going to have a good time. Follow this gist for instructions on how to do the conversion: COMING SOON.

Once you are sure about which branch you plan to install, select it from the drop down menu located at the top left of the page.

![Step1](/step1_branch.png)

Once you have selected a branch you will have access to the source code (Marlin only) as well as the compiled binary file (the firmware that you need to put on your SD card) for that branch. Many of you will not care about the source code and are here for the compiled firmware but I made the source available anyways. To download the firmware for Marlin simply follow the steps in the next pictues.

![Step2](/step2_firmware.png)

![Step2](/step3_download.png)

Finally, locate the TFT zip folder in the same file listing and download it (if there is no TFT folder then it means that it is not required for that particular update).

## Using the firmware on the B1

If you are not using a B1 then you can skip this part and find the sub-heading that covers the printer that you are using.

By this point you should have both the TFT and the Marlin firmware downloaded. Before installing the Marlin firmware, make sure that you have read any gists linked to that firmware and take note of the following points:

1. The ABL versions of this firmware use the probe as the z-endstop. I prefer this method over using a dedicated z-endstop switch. This means that you will need to remove the z-endstop post that is screwed into the back of the left, upright 4020 extrusion.
2. The ABL versions of this firmware are programmed to apply any z babysteps to the z-offset. This means that if you save your babysteps the nozzle will always start in the right place for each print regardless of what you do to your bed provided that you don't mess with the position of the BL touch between prints (e.g. take apart the hotend).
3. The ABL versions of this firmware assume that the probe is located in the position when using the stock mount. If you are using the mount by @thisiskeithb from thingiverse that places the probe in the front-center of the hotend mount then you will need to adjust your z-probe offsets using the LCD. Using the unified interface nagivate to Menu --> Settings --> Machine --> Parameter --> Probe Offset and change the values to: X = -1.5 Y = -34 Z = -1.5.

**To install Marlin:** once you have downloaded the firmware file just copy it onto your SD card **in the motherboard (not TFT)** and reboot. Once it is done you should have a FIRMWARE.CUR file on the SD. That means it worked.

If you use an ABL version you will receive an error message relating to EEPROM as soon as you boot up. This is normal. Simply enter the terminal and send M500 in order to clear it.

**To install the TFT firmware:** simply unzip the contents of the zip folder and copy them onto an SD card. Put the SD card into **the TFT SD slot** and reboot. You will see a series of images on the TFT showing you the status of the update. Do not ignore any errors as doing so will result in your TFT freezing when trying to use it. Rather, try the installation again and if it fails a second time then report the issue in the issue log.

I recommend performing a home on all axes directly after installing the firmware and using your finger to trigger the probe on the way down for the z home. This will confirm that the probe is working fine and prevent the nozzle from crashing into the bed if it is not.

## Using the firmware on the BX

If you are not using a BX then you can skip this part and find the sub-heading that covers the printer that you are using.

In addition to the section below, make sure that you read the points in this gist. They explain how to get the most of this firmware: https://gist.github.com/looxonline/94510f433c0f40ddcacbbecc384e844f

By this point you should have the TFT graphics (optional) and the Marlin firmware downloaded. Before installing it, make sure that you have read any gists linked to that firmware and take note of the following points:

1. All versions of this firmware use the ABL probe in place of the z-endstop switch. There is no need for you to change any wiring but you will need to physically remove the z-endstop switch from the right upright and unplug it. **Unplugging it alone is not enough as it can still physically get in the way.** Don't be afraid when you see your nozzle heading towards the bed. As long as you were able to successfully perform ABL probing before the upgrade your nozzle will not hit the bed. Why do I say that it is better to use the machine without a z-endstop? Well, imagine that you home your z-axis and the nozzle is just a touch too high for the first layer so you decide that you will babystep it down. The problem is that when you homed the z-axis it was already at its minimum and hitting against the endstop switch. Babystepping it down will just ram it hard against the switch and cause strain to your z-axis. It's better to remove the switch and use the probe as the z limit since babystepping will work just fine thereafter.
2. All versions of this firmware support the silent fan modification (see opening gist) but you don't **have** to do the mod if you want to use the firmware. It will work either way.
3. This firmware adjusts the speed at which homng takes place as well as the current that is provided to the motors during homing. Since there are no endstops on X and Y you will need to recalibrate your sensorless homing sensitivity for each axis to account for the changes. Read the "BX getting started" gist for instructions on how to do this.
4. After installing the firmware the machine will beep a lot and throw a lot of error messages. This is absolutely normal and nothing to worry about. Simply reset the machine and it will be back to normal.

**To install the firmware:** simply copy the firmware.bin, config.ini and TFT70 files (if the config.ini file and TFT70 folder are not there it means that they are not needed for that update) and folders onto the root of a FAT32 formatted SD card. Insert the card into the front facing SD card slot on the BX. Technically both will work but when installing firmware it is best to minimise the possibility of bit errors and the SD card closest to the CPU is the one that will do this. Reboot the machine and the firmware will update. Once the firmware update is complete, make sure that firmware.bin has changed to FIRMWARE.CUR and config.ini has changed to CONFIG.CUR.

## Other mods

Here is a list of useful mods that you may want to perform on your machine. If there is a mod that you are aware of and feel that it should be added here then please feel free to submit it.

First for the BX:

Mod Name | Mod Link
------------ | -------------
BX Silent fan mod | https://gist.github.com/looxonline/2966862620b831a33cb36340ed73f644
BX USB free pi integration | https://gist.github.com/looxonline/89e79b2554771eee8aa8b6492f30400d
BX Pi safe, auto shutdown | https://gist.github.com/looxonline/fd260bfc29c124f22fe7613311ae3a79

B1 Mod list coming soon...

## Want to help?

If you encounter an issue with the firmware or any of the instructions here then please submit an issue using github.

I am also looking for experienced users who are willing to test new versions of the firmware on their machines before being released to a wider audience. If you would like to become a tester then please send me a PM on facebook using the name "Luke Jobs Harrison".

## License

See upstream licenses for source code from Marlin and the BTT TFT.
