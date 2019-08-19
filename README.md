## Creality CR10S Pro, CR-X, CR10 Max and Ender 5 Plus Touch Screen supported firmware

The firmware branches maintained here are made possible by the support of sponsors like [[Tiny Machines 3D](https://tinymachines3d.com/) as well as our customer base through our 3D printing Services. Maintaining and developing these branches takes a significant investment, made up of time and machines. To support continued development, please consider your next 3D Printer related purchase from Tiny Machines 3D and thank them for supporting open source development. Or, consider us for printing services outside of your machine’s capabilities. If you do not need anything printed or a 3D Printer but still want to contribute, you can support us through [[Patreon](https://www.patreon.com/InsanityAutomation)].


## Creality Firmware Branches
  - 1.1.9 Firmware (Stable, EOL) [[TM_CR10](https://github.com/InsanityAutomation/Marlin/tree/TM_CR10)]
  - 2.0 Firmware (Beta, but well tested) [[Creality_Marlin2.0.x](https://github.com/InsanityAutomation/Marlin/tree/Creality_Marlin2.0.x)]
  - 2.0 Development Branch [[Creality_2.0_Devel](https://github.com/InsanityAutomation/Marlin/tree/Creality_2.0_Devel)]
  - Touchscreen Firmware (10SPro and CR-X 1.1.9 Stable) [[Creality_DWINTest](https://github.com/InsanityAutomation/Marlin/tree/Creality_DWINTest)]
  - Touchscreen Firmware (10SPro and CR-X 2.0 Active Development) [[CrealityDwin_2.0](https://github.com/InsanityAutomation/Marlin/tree/CrealityDwin_2.0)]
## Formbot / Vivedino Firmware Branches
  - Raptor 1 1.1.9 Firmware [[TM_Raptor](https://github.com/InsanityAutomation/Marlin/tree/TM_Raptor)]
  - Raptor 1/2 2.0 Firmware [[Raptor_2.0.X](https://github.com/InsanityAutomation/Marlin/tree/Raptor_2.0.X)]
  - Trex 2+/3 1.1.9 Firmware [[Trex3_1.1.9](https://github.com/InsanityAutomation/Marlin/tree/Trex3_1.1.9)]
  - Trex 2+/3 2.0 Firmware (Be warned of layer shift issue below!) [[TM_Trex2+_2.0.x(https://github.com/InsanityAutomation/Marlin/tree/TM_Trex2+_2.0.x)]
## Mamorubot / HieHa Firmware Branches
  - SX4 1.1.9 Firmware (Stable, EOL) [[Mamorubot_SX4]( https://github.com/InsanityAutomation/Marlin/tree/Mamorubot_SX4)]
  - SX4 2.0 Firmware (Beta, but well tested) [[TM_SX4_2.0](https://github.com/InsanityAutomation/Marlin/tree/TM_SX4_2.0)]
  - SX4 Development Branch [[TM_SX4_2.0_Devel](https://github.com/InsanityAutomation/Marlin/tree/TM_SX4_2.0_Devel)]
## Other Firmware
  - Raise 3D N2+ (Dual) 2.0 [[Raise3D-N2+-Dual](https://github.com/InsanityAutomation/Marlin/tree/Raise3D-N2+-Dual)]

[[Marlin Commit History](https://github.com/MarlinFirmware/Marlin/pulls?q=is%3Apr+is%3Aclosed+author%3AInsanityAutomation)]

## Active Projects
  - [[Tracing Marlin 2.0 Planner bug causing layer Shifting] (https://github.com/MarlinFirmware/Marlin/issues/12403)]
  - Creality Dwin Touchscreen rewrite to ExtUI
  - [[IDEX drifting z offset on tool change with UBL]( https://github.com/MarlinFirmware/Marlin/issues/13817)]
  - [[Multiple touch homing] (https://github.com/MarlinFirmware/Marlin/issues/9802)]
  - [[Tool change offset changes] (https://github.com/MarlinFirmware/Marlin/issues/12568)]
  - [[Hotend offset adjustment clamping] (https://github.com/MarlinFirmware/Marlin/pull/13669)]
  - [[Filament width sensor hang on 32 bit] (https://github.com/MarlinFirmware/Marlin/issues/13701)]
  - Probe point output for all leveling types matching incomplete ubl implementation
  - SKR1.1 Pro integration to Trex2 with TMC2209 Drivers
  - SKR1.3 Integration to Mamorubot SX4
  - Mosquito BMG-M integration to Raptor
  - Porting Chithu Touchscreen board to Marlin 2.0 with ExtUI
  - Anet A10 Delta STM32F103 board analysis
  - Smoothing integration and vendor components of Marlin 2.0 (Revision to version.h handling, limit clamping, usability enhancements ect)
  - Consolidate more probing functions, improve G33/4 and G28/29 handling
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
This is just one of many forks of Marlin. We dont try to bury that behind fancy marketting or anything else. As you can see from the links above, most of the work done here is submitted back to the mainstream Marlin branches. The end goal of every project is to get it to the point where it is merged and maintained there. See marlin resources, including how to contribute to the Marlin Project as well, down below.

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
