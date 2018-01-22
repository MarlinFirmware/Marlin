# This HAL is for the STM32F765 board "The Borg" used with STM32Generic Arduino core by danieleff.

# Original core is located at:

https://github.com/danieleff/STM32GENERIC

but i have not committed the changes needed for the Borg there yet, so please use:

https://github.com/Spawn32/STM32GENERIC

Unzip it into [Arduino]/hardware folder


Download the latest GNU ARM Embedded Toolchain:

https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

(The one in Arduino dosen't support STM32F7).

Change compiler.path in platform.txt to point to that you downloaded.

# This HAL is in development.
# Currently only tested on "The Borg".

You will also need the latest Arduino 1.9.0-beta or newer.

This HAL is a modified version of Chris Barr's Picoprint STM32F4 HAL, so shouldn't be to hard to get it to work on a F4.

