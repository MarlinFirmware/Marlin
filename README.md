# Marlin 3D Printer Firmware

[![Badge License]][License]
[![Badge Contributors]][Contributors]
![Badge Release Date]
[![Badge CI]][CI]

<img align="right" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

Additional documentation can be found at the [Marlin Home Page][Website].
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

## Marlin 2.0

Marlin 2.0 takes this popular RepRap firmware to the next level by adding support for much faster 32-bit and ARM-based boards while improving support for 8-bit AVR boards. Read about Marlin's decision to use a "Hardware Abstraction Layer" below.

Download earlier versions of Marlin on the [Releases page][Releases].

## Example Configurations

Before building Marlin you'll need to configure it for your specific hardware. Your vendor should have already provided source code with configurations for the installed firmware, but if you ever decide to upgrade you'll need updated configuration files. Marlin users have contributed dozens of tested example configurations to get you started. Visit the [MarlinFirmware/Configurations][Configurations] repository to find the right configuration for your hardware.

## Building Marlin 2.0

To build Marlin 2.0 you'll need [Arduino IDE 1.8.8 or newer][Arduino] or [PlatformIO]. Detailed build and install instructions are posted at:

  - [Installing Marlin (Arduino)][Installation Arduino]
  - [Installing Marlin (VSCode)][Installation VSCode].

### Supported Platforms

  Platform|MCU|Example Boards
  --------|---|-------
  [Arduino AVR][Platform AVR]|ATmega|RAMPS, Melzi, RAMBo
  [Teensy++ 2.0][Platform Teensy++ 2.0]|AT90USB1286|Printrboard
  [Arduino Due][Platform Due]|SAM3X8E|RAMPS-FD, RADDS, RAMPS4DUE
  [ESP32][Platform ESP32]|ESP32|FYSETC E4, E4d@BOX, MRR
  [LPC1768][Platform LPC1768]|ARM® Cortex-M3|MKS SBASE, Re-ARM, Selena Compact
  [LPC1769][Platform LPC1769]|ARM® Cortex-M3|Smoothieboard, Azteeg X5 mini, TH3D EZBoard
  [STM32F103][Platform STM32F103]|ARM® Cortex-M3|Malyan M200, GTM32 Pro, MKS Robin, BTT SKR Mini
  [STM32F401][Platform STM32F401]|ARM® Cortex-M4|ARMED, Rumba32, SKR Pro, Lerdge, FYSETC S6
  [STM32F7x6][Platform STM32F7x6]|ARM® Cortex-M7|The Borg, RemRam V1
  [SAMD51P20A][Platform SAMD51P20A]|ARM® Cortex-M4|Adafruit Grand Central M4
  [Teensy 3.5][Platform Teensy 3.5]|ARM® Cortex-M4|
  [Teensy 3.6][Platform Teensy 3.6]|ARM® Cortex-M4|
  [Teensy 4.0][Platform Teensy 4.0]|ARM® Cortex-M7|
  [Teensy 4.1][Platform Teensy 4.1]|ARM® Cortex-M7|
  Linux Native|x86/ARM/etc.|Raspberry Pi

## Submitting Changes

- Submit **Bug Fixes** as Pull Requests to the ([bugfix-2.0.x]) branch.
- Follow the [Coding Standards] to gain points with the maintainers.
- Please submit your questions and concerns to the [Issue Queue][Issues].

## Marlin Support

The Issue Queue is reserved for Bug Reports and Feature Requests. To get help with configuration and troubleshooting, please use the following resources:

- [Marlin Documentation](http://marlinfw.org) - Official Marlin documentation
- [Marlin Discord][Discord] - Discuss issues with Marlin users and developers
- Facebook Group ["Marlin Firmware"][Facebook]
- RepRap.org [Marlin Forum][Forum]
- Facebook Group ["Marlin Firmware for 3D Printers"][Facebook 3D]
- [Marlin Configuration][YouTube] on YouTube

## Contributors

Marlin is constantly improving thanks to a huge number of contributors from all over the world bringing their specialties and talents. Huge thanks are due to [all the contributors][Contributors] who regularly patch up bugs, help direct traffic, and basically keep Marlin from falling apart. Marlin's continued existence would not be possible without them.

## Administration

Regular users can open and close their own issues, but only the administrators can do project-related things like add labels, merge changes, set milestones, and kick trolls. The current Marlin admin team consists of:

 - Scott Lahteine [[@thinkyhead]] - USA - Project Maintainer <br>
   [Donate][Donate Scott]
 - Roxanne Neufeld [[@Roxy-3D]] - USA
 - Keith Bennett [[@thisiskeithb]] - USA
 - Victor Oliveira [[@rhapsodyv]] - Brazil
 - Chris Pepper [[@p3p]] - UK
 - Jason Smith [[@sjasonsmith]] - USA
 - Luu Lac [[@shitcreek]] - USA
 - Bob Kuhn [[@Bob-the-Kuhn]] - USA
 - Erik van der Zalm [[@ErikZalm]] - Netherlands<br>
   [Donate][Donate Erik]

## License

Marlin is published under the [GPL license][License] because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.

<!----------------------------------------------------------------------------->

[Badge Contributors]: https://img.shields.io/github/contributors/marlinfirmware/marlin.svg
[Badge Release Date]: https://img.shields.io/github/release-date/marlinfirmware/marlin.svg
[Badge License]: https://img.shields.io/badge/License-GPLv3-blue.svg
[Badge CI]: https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x

[License]: LICENSE


[CI]: https://github.com/MarlinFirmware/Marlin/actions
[Contributors]: https://github.com/MarlinFirmware/Marlin/graphs/contributors
[Releases]: https://github.com/MarlinFirmware/Marlin/releases
[bugfix-2.0.x]: https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x
[Issues]: https://github.com/MarlinFirmware/Marlin/issues


[PlatformIO]: https://docs.platformio.org/en/latest/ide.html#platformio-ide
[Website]: https://marlinfw.org/
[Arduino]: https://www.arduino.cc/en/main/software
[Configurations]: https://github.com/MarlinFirmware/Configurations
[Coding Standards]: http://marlinfw.org/docs/development/coding_standards.html


[Installation Arduino]: https://marlinfw.org/docs/basics/install_arduino.html
[Installation VSCode]: https://marlinfw.org/docs/basics/install_platformio_vscode.html


[Discord]: https://discord.gg/n5NJ59y
[Facebook]: https://www.facebook.com/groups/1049718498464482/
[Forum]: https://forums.reprap.org/list.php?415
[Facebook 3D]: https://www.facebook.com/groups/3Dtechtalk/
[YouTube]: https://www.youtube.com/results?search_query=marlin+configuration


[Platform AVR]: https://www.arduino.cc/
[Platform Teensy++ 2.0]: http://www.microchip.com/wwwproducts/en/AT90USB1286
[Platform Due]: https://www.arduino.cc/en/Guide/ArduinoDue
[Platform ESP32]: https://github.com/espressif/arduino-esp32
[Platform LPC1768]: http://www.nxp.com/products/microcontrollers-and-processors/arm-based-processors-and-mcus/lpc-cortex-m-mcus/lpc1700-cortex-m3/512kb-flash-64kb-sram-ethernet-usb-lqfp100-package:LPC1768FBD100
[Platform LPC1769]: https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1700-cortex-m3/512kb-flash-64kb-sram-ethernet-usb-lqfp100-package:LPC1769FBD100
[Platform STM32F103]: https://www.st.com/en/microcontrollers-microprocessors/stm32f103.html
[Platform STM32F401]: https://www.st.com/en/microcontrollers-microprocessors/stm32f401.html
[Platform STM32F7x6]: https://www.st.com/en/microcontrollers-microprocessors/stm32f7x6.html
[Platform SAMD51P20A]: https://www.adafruit.com/product/4064
[Platform Teensy 3.5]: https://www.pjrc.com/store/teensy35.html
[Platform Teensy 3.6]: https://www.pjrc.com/store/teensy36.html
[Platform Teensy 4.0]: https://www.pjrc.com/store/teensy40.html
[Platform Teensy 4.1]: https://www.pjrc.com/store/teensy41.html


[@thisiskeithb]: https://github.com/thisiskeithb
[@Bob-the-Kuhn]: https://github.com/Bob-the-Kuhn
[@sjasonsmith]: https://github.com/sjasonsmith
[@thinkyhead]: https://github.com/thinkyhead
[@rhapsodyv]: https://github.com/rhapsodyv
[@shitcreek]: https://github.com/shitcreek
[@ErikZalm]: https://github.com/ErikZalm
[@Roxy-3D]: https://github.com/Roxy-3D
[@p3p]: https://github.com/p3p


[Donate Scott]: https://www.thinkyhead.com/donate-to-marlin
[Donate Erik]: https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software