# Tatara Anet A8 SKR 1.4 Turbo TMC 2209 BLTouch
This is my version of Marlin 2.6 for my new Tatara framed Anet A8. It should be a good basis for similar printers, though I did have trouble with my stepper directions.

To get the BLTouch working I used this fix:
Open Conditionals_LCD.h located in ..Marlin\src\inc.. and change this line:
#if Z_HOME_DIR < 0 && !HAS_CUSTOM_PROBE_PIN
to this:
#if Z_HOME_DIR < 0 // && !HAS_CUSTOM_PROBE_PIN

From <https://github.com/bigtreetech/BIGTREETECH-SKR-V1.3/issues/207> 



# Marlin 3D Printer Firmware

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
[![Build Status](https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/MarlinFirmware/Marlin/actions)

## Tatara: like an Anet A8

But with a steel frame. This one uses an SKR 1.4 turbo with 5 TMC2009 drivers and a BLTouch probe.

Only 3 files are changes from the Marlin 2.5 bugfix
Configuration.h, Configuration_adv.h and Conditionals_LCD.h


## For the BLTouch
Open Conditionals_LCD.h located in ..Marlin\src\inc.. and change these lines:

  #if Z_HOME_DIR < 0  && (!HAS_CUSTOM_PROBE_PIN || ENABLED(USE_PROBE_FOR_Z_HOMING))
  to
  #if Z_HOME_DIR < 0 // && (!HAS_CUSTOM_PROBE_PIN || ENABLED(USE_PROBE_FOR_Z_HOMING))

From <https://github.com/bigtreetech/BIGTREETECH-SKR-V1.3/issues/207> 