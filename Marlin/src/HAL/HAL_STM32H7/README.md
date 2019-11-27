# This HAL is for the STM32H743ZI (Nucleo Board)

## Original core is located at:

  - https://github.com/danieleff/STM32GENERIC

But I haven't committed the changes needed for the Borg there yet, so please use:

  - https://github.com/Spawn32/STM32GENERIC
    Unzip this into the [Arduino]/hardware folder

Download the latest GNU ARM Embedded Toolchain:

  - https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
    (The one in Arduino doesn't support STM32H7).

Change compiler.path in platform.txt to point to the folder you downloaded.

## This HAL is in development.
## Currently only tested on "The Borg".

You will also need the latest Arduino 1.9.0-beta or newer.

This HAL is a modified version of Chris Barr's Picoprint STM32F4 HAL, so shouldn't be too hard to adapt to the F4.
