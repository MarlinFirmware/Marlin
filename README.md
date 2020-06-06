## Creality CR10S Pro, CR-X, CR10 Max and Ender 5 Plus Touch Screen supported firmware

This branch adds support for the Creality touchscreen machines and was split off from the standard Creality branches due to specific custom changes not found upstream. This includes extensions to the EXTui framework among other smaller tweaks that we have been working towards getting submitted upstream. Machines supported here are :

- [CR10S Pro](http://bit.ly/2rxZ6lP)
- [CR10 Max](http://bit.ly/2L6wZRy)
- [CR-X](http://bit.ly/2L6xsmM)
- [Ender 5 Plus](https://amzn.to/37TE8yH)
- [CR10 V2 ](http://bit.ly/37PaXfV)
- [CR20 and Pro](http://bit.ly/2Oxxruk)
- [CR10S](http://bit.ly/33AAaHv)
- [CR10S4 400mm](http://bit.ly/2XXCBmq)
- [CR10S5 500mm](http://bit.ly/35K4ZuR)

The CRX Has screen files with or without ABL. All other machines should use the same generic set. V2 is the current screen file version.

Screen files are archived with [7-Zip](https://www.7-zip.org/) simply because it came out 1/5 the file size of a zip file. That added up fast!

Now that we have configured an auto build, we are able to provide quite a few HEX file configurations. There is an issue with Windows systems limiting path depth however so the file names need to be shorter than we would prefer. Therefore, here is a legend to decoding what they are :

- BLT = BLTouch
- BIL = Bilinear Leveling
- UBL - Unified Bed Leveling
- DZ = Dual Z Steppers
- Fil = FilamentRunout
- Slnt = Creality Silent Board
- H = E3D Hemera Extruder
- MC = Mosquito Creality mount
- ME = Mosquito E3D mount
- Melzi Host option disables local SD card to allow more features and buffer for Octoprint control
- NF = Noise filtering for machines with cable extensions - reduces homing accuracy!

## Primary notes for DW6
- Power Loss Recovery is functional again
- Added screen for manual PID entry and custom tuning
- Added screen for Step/mm and probe offset editing
- Base bumped to Marlin 2.0.4.4
- Tweaks to limit RAM consumption where possible

The default build button is now a batch Build All! If you want to build a custom stock 2560 environment, use the env:megaatmega2560 environment. For SKR boards use the LPC1768/9 environments as usual.

## About Our Branches

The firmware branches maintained here are made possible by the support of sponsors like [Tiny Machines 3D](https://www.tinymachines3d.com/?rfsn=3419592.cc302fe) as well as our customer base through our 3D printing Services. Maintaining and developing these branches takes a significant investment, made up of time and machines. To support continued development, please consider your next 3D Printer related purchase from Tiny Machines 3D and thank them for supporting open source development. Or, consider us for printing services outside of your machine’s capabilities. Print service requests can be sent to d.menzel@insanityautomation.com and we will respond typically within 1 working day. If you do not need anything printed or a 3D Printer but still want to contribute, you can support us through [Patreon](https://www.patreon.com/InsanityAutomation).

## Setup

All configuration options intended to be adjusted by end users have been placed in the top section of Configuration.h and have been documented there. There is typically a break line to segregate the standard configuration below. Anything aside from the upper options is intended for advanced users only.

## Known Issues
 - While auto leveling (measuring) is in progress pressing other buttons on the screen can abort portions of the script depending where it is
 - - The process includes heating the bed before probing, probing then heating the nozzle before moving to Z0.

 - UBL functions differently than Bilinear. It currently probes 16 points and extrapolates where the probe cannot reach. This is because UBL is designed to build a mesh everywhere the nozzle can reach, where bilinear only works where the probe can reach. These points are intended to be manually tuned however the touchscreen has no interface to do so. We are working on a way to increase the mesh density and feed alternative points to the screen. This is especially important for the CR-10 Max. Please mind the differences with UBL during use.

 - There are some points where the screen accepts input before it's really ready to (Measuring on Auto Leveling for example, wait for message to Set Z Offset) so please be patient! Sending inputs (pressing buttons) essentially overwrites the existing command with the new one.

 - CR-X Non-Auto Leveling screens have the start of baby-stepping abilities on the adjust screen, however this has not been fully tested yet. Use with caution.


## Support

This firmware is provided to the public as-is with no warranty or guarantee. It's based on a large open source project and there is no entitlement to support. That said, Tiny Machines 3D customers may obtain support through their normal support queue. I will provide support to Patreons as I am able. If you require more immediate support and are not a Tiny Machines customer, you may contact them at support@tinymachines3d.com about purchasing support hours. Aside from this, we are active many Facebook groups as well as several discord channels and typically help anyone we can when we come across them.

3D Printing Discord - https://discord.gg/MZrsgVU
Marlin Discord - https://discord.gg/n5NJ59y

## Future Goals

For this branch, we still have some active goals open that we plan to continue working on provided there is continued interest in the project.
- SD Card sub directory traversal
- Volume and leveling state EEPROM storage
- Utilize a few more of the background screens (such as print finished) that are currently not in use but not necessary


## Firmware Branches
  - 1.1.9 Firmware (Stable, EOL) [TM_CR10](https://github.com/InsanityAutomation/Marlin/tree/TM_CR10)
  - 2.0 Firmware [Creality_Marlin2.0.x](https://github.com/InsanityAutomation/Marlin/tree/Creality_Marlin2.0.x)
  - 2.0 Development Branch (Beta, but well tested) [Creality_2.0_Devel](https://github.com/InsanityAutomation/Marlin/tree/Creality_2.0_Devel)
  - 2.0 Bleeding edge Branch (Here there be dragons!) [Creality_2.0_Bleeding](https://github.com/InsanityAutomation/Marlin/tree/Creality_2.0_Bleeding)
  - Touchscreen Firmware (10SPro and CR-X 1.1.9 Stable) [Creality_DWINTest](https://github.com/InsanityAutomation/Marlin/tree/Creality_DWINTest)
  - Touchscreen Firmware (10SPro and CR-X 2.0 Active Development) [CrealityDwin_2.0](https://github.com/InsanityAutomation/Marlin/tree/CrealityDwin_2.0)
## Formbot / Vivedino Firmware Branches
  - Raptor 1 1.1.9 Firmware [TM_Raptor](https://github.com/InsanityAutomation/Marlin/tree/TM_Raptor)
  - Raptor 1/2 2.0 Firmware [Raptor_2.0.X](https://github.com/InsanityAutomation/Marlin/tree/Raptor_2.0.X)
  - Trex 2+/3 1.1.9 Firmware [Trex3_1.1.9](https://github.com/InsanityAutomation/Marlin/tree/Trex3_1.1.9)
  - Trex 2+/3 2.0 Firmware (Be warned of layer shift issue below!) [TM_Trex2+_2.0.x](https://github.com/InsanityAutomation/Marlin/tree/TM_Trex2+_2.0.x)
## Mamorubot / HieHa Firmware Branches
  - SX4 1.1.9 Firmware (Stable, EOL) [Mamorubot_SX4]( https://github.com/InsanityAutomation/Marlin/tree/Mamorubot_SX4)
  - SX4 2.0 Firmware (Beta, but well tested) [TM_SX4_2.0](https://github.com/InsanityAutomation/Marlin/tree/TM_SX4_2.0)
  - SX4 Development Branch [TM_SX4_2.0_Devel](https://github.com/InsanityAutomation/Marlin/tree/TM_SX4_2.0_Devel)
## Other Firmware
  - Raise 3D N2+ (Dual) 2.0 [Raise3D-N2+-Dual](https://github.com/InsanityAutomation/Marlin/tree/Raise3D-N2+-Dual)
  - Evnovo Artillery Sidewinder X1 2.0 [Evnovo X1](https://github.com/InsanityAutomation/Marlin/tree/ArtilleryX1_2.0_Devel)

[Marlin Commit History](https://github.com/MarlinFirmware/Marlin/pulls?q=is%3Apr+is%3Aclosed+author%3AInsanityAutomation)

## Active Projects
  - [Tracing Marlin 2.0 Planner bug causing layer Shifting](https://github.com/MarlinFirmware/Marlin/issues/12403)
  - Creality Dwin Touchscreen rewrite to ExtUI - DONE
  - [IDEX drifting z offset on tool change with UBL](https://github.com/MarlinFirmware/Marlin/issues/13817)
  - [Multiple touch homing](https://github.com/MarlinFirmware/Marlin/issues/9802)
  - [Tool change offset changes](https://github.com/MarlinFirmware/Marlin/issues/12568)
  - [Hotend offset adjustment clamping](https://github.com/MarlinFirmware/Marlin/pull/13669)
  - [Filament width sensor hang on 32 bit](https://github.com/MarlinFirmware/Marlin/issues/13701)
  - Probe point output for all leveling types matching incomplete ubl implementation - DONE
  - SKR1.1 Pro integration to Trex2 with TMC2209 Drivers - Mostly done, blew an output...
  - SKR1.3 Integration to Mamorubot SX4 - DONE
  - Mosquito BMG-M integration to Raptor
  - Porting Chithu Touchscreen board to Marlin 2.0 with ExtUI
  - Anet A10 Delta STM32F103 board analysis
  - Smoothing integration and vendor components of Marlin 2.0 (Revision to version.h handling, limit clamping, usability enhancements ect)
  - Consolidate more probing functions, improve G33/4 and G28/29 handling
  - G34 Z stepper reordering and automatic detection / adjustment of reversed stepper connections
  - Video documentation of firmware branch configuration options and flashing with platformio
## Planned Projects
  - Implement Virtual Tools
  - Port Duet2 Wifi with Duex5 to Marlin 2.0
  - Further Gcode Parity with RRF
  - Extend Host Prompt Support for Power Loss Recovery
  - Improve native ESP32 web support
  - Port M425 Auto backlash and offset calibration to IDEX
  - Integrate 12 stepper LPC1778 PCB to configure ideXY quad head system as well as integrate 3 extruder mixing to IDEX
  - Video walkthrough of each bed leveling system usage and all functions

## Marlin
This is just one of many forks of Marlin. We don't try to bury that behind fancy marmarketingketting or anything else. As you can see from the links above, most of the work done here is submitted back to the mainstream Marlin branches. The end goal of every project is to get it to the point where it is merged and maintained there. See marlin resources, including how to contribute to the Marlin Project as well, down below.

## Building Marlin 2.0

To build Marlin 2.0 you'll need [Arduino IDE 1.8.8 or newer](https://www.arduino.cc/en/main/software) or [PlatformIO](http://docs.platformio.org/en/latest/ide.html#platformio-ide). We've posted detailed instructions on [Building Marlin with Arduino](http://marlinfw.org/docs/basics/install_arduino.html) and [Building Marlin with PlatformIO for ReArm](http://marlinfw.org/docs/basics/install_rearm.html) (which applies well to other 32-bit boards).


## The current Marlin dev team consists of:

 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)] - USA &nbsp; [![Flattr Scott](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=thinkyhead&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)] - USA
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)] - USA
 - Chris Pepper [[@p3p](https://github.com/p3p)] - UK
 - João Brazio [[@jbrazio](https://github.com/jbrazio)] - Portugal
 - Erik van der Zalm [[@ErikZalm](https://github.com/ErikZalm)] - Netherlands &nbsp; [![Flattr Erik](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
