# Marlin 3D Printer Firmware for BIQU B1

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
[![Build Status](https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/MarlinFirmware/Marlin/actions)

<img align="right" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

You are here because you are looking for the latest BIQU B1 firmware and you will hopefully find what you are looking for! Read below to find out how.

## Current versions of Marlin and TFT

The files in this repository contain the source, configs and compiled firmware for Marlin: 2.0.7.2

The firmware within this repo uses modified versions of the standard Marlin config files created by the talented @thisiskeithb.

You will also find the latest compiled versions of the TFT firmware which has been customized for use with the B1.

## Using this repo.

People love to add on to their printers. For this reason there cannot be just a single Marlin firmware that satisfies all of the variations of B1 out there. This repo aims to create firmware for the most popular variants and keep it up to date with the latest Marlin releases.

Each B1 variant is stored in a branch. You don't need to know much about github to know how to select the correct branch for your printer. It's really easy. Just select the name that best matches your variant of the B1 from the drop down list and you will be on the correct branch.

The available branches or B1 variants are:

Branch Name | Variant Properties
------------ | -------------
B1_STOCK | If you have built your B1 out of the box and done nothing else to it then this is the branch for you.
B1_ABL | If you have added an ABL sensor (BL Touch or other) then this is the branch for you. This does a 3x3 probe on the bed which is not the highest resolution but is often enough for beds that are not badly warped.
B1_ABL_HIGH_RES | If you have added an ABL sensor (BL Touch or other) and you want a higher res on the bed probing because of slightly more warping and are happy to pay a small time penalty at the start of each print then this is for you. This does a 4 x 4 probe.
B1_ABL_SUPER_RES | If you have added an ABL sensor (BL Touch or other) and you want an incredibly high res probe of the bed and are willing to wait a few minutes extra at the start of each print then this is for you.  This does a 5 x 5 probe.
B1_ABL_SFS | Coming soon...

![Step1](/step1_branch.png)

Once you have selected a branch you will have access to the source code (Marlin only) as well as the compiled binary file (the firmware that you need to put on your SD card) for that branch. Most of you will not care about the source code and are here for the compiled firmware but I made the source available anyways. To download the firmware for Marlin simply follow the steps in the next pictues.

![Step2](/step2_firmware.png)

![Step2](/step3_download.png)

To download the TFT firmware simply locate the TFT zip file in the root directory and download it. Instructions on how to install both follow.

## Using the Firmware

Before installing the Marlin firmware take note of the following points:

1. The ABL versions of this firmware use the probe as the z-endstop. I prefer this method over using a dedicated z-endstop switch. This means that you will need to remove the z-endstop post that is screwed into the back of the left, upright 4020 extrusion.
2. The ABL versions of this firmware are programmed to apply any z babysteps to the z-offset. This means that if you save your babysteps the nozzle will always start in the right place for each print regardless of what you do to your bed provided that you don't mess with the position of the BL touch between prints (e.g. take apart the hotend).
3. The ABL versions of this firmware assume that the probe is located in the position when using the stock mount. If you are using the mount by @thisiskeithb from thingiverse that places the probe in the front-center of the hotend mount then you will need to adjust your z-probe offsets using the LCD. Using the unified interface nagivate to Menu --> Settings --> Machine --> Parameter --> Probe Offset and change the values to: X = -1.5 Y = -34 Z = -1.5.

To install Marlin: once you have downloaded the firmware file just copy it onto your SD card in the motherboard (not TFT) and reboot. Once it is done you should have a FIRMWARE.CUR file on the SD. That means it worked.

If you use an ABL version you will receive an error message relating to EEPROM as soon as you boot up. This is normal. Simply enter the terminal and send M500 in order to clear it.

To install the TFT firmware simply unzip the contents of the zip folder and copy them onto an SD card. Put the SD card into the TFT SD slot and reboot. You will see a series of images on the TFT showing you the status of the update. Do not ignore any errors as doing so will result in your TFT freezing when trying to use it. Rather, try the installation again and if it fails a second time then report the issue in the issue log.

I recommend performing a home on all axes directly after installing the firmware and using your finger to trigger the probe on the way down for the z home. This will confirm that the probe is working fine and prevent the nozzle from crashing into the bed if it is not.

## Want to help?

If you'd like to make futher tweaks to the configs then you are more than welcome to submit issues or pull requests.

Hopefully we will be able to grow this repo to include configs and builds for a variety of common implementations of the B1 which will save people a lot of frustration.

## Supported Platforms

BIQU B1 With others coming soon (BX - Here's looking at you sexy)...

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open o...

While we can't prevent the us....bla...bla...bla
