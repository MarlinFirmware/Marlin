# Marlin for the Voxlab Aquila X2 (H32)

This project aims to port vanilla Marlin to the [Voxlab Aquila X2](https://www.voxelab3dp.com/product/aquila-x2-fdm-3d-printer) 3D-Printer with H32 (HC32F46x) SoC.

⚠ before doing anything, read the **FULL** readme first ⚠

this project is based on the following projects and wouldn't have been possible without them:

- [shadow578/platform-hc32f46x](https://github.com/shadow578/platform-hc32f46x) (platformio platform for HC32F46x)
- [shadow578/framework-arduino-hc32f46x](https://github.com/shadow578/framework-arduino-hc32f46x) (Arduino framework for HC32F46x)
- [MarlinFirmware/Marlin](https://github.com/MarlinFirmware/Marlin) (base Marlin firmware)
- [Voxelab-64/Aquila_X2](https://github.com/Voxelab-64/Aquila_X2) (platform, framework and HAL)
- [alexqzd/Marlin-H32](https://github.com/alexqzd/Marlin-H32) (optimizations to framework and `HAL`)
- [stm32duino/Arduino_Core_STM32](https://github.com/stm32duino/Arduino_Core_STM32) (misc. Arduino functions)

details on the origin of code used is described in README files accompanying the components.

# Forking this Project

if you wish to fork this project, please keep the following in mind:

1. create a proper fork of this repository, and not a copy. this makes it easier to keep track of changes.

- you can do this by clicking the "fork" button in the top-right corner of this page.
- if your repo does **not** show a little "forked from" message in the top-left corner, you've done it wrong.

2. if you make improvements, please consider creating a PR to this repo. to make it easier to merge, please keep the following in mind:

- avoid changing files outside of the HAL as much as possible. doing so makes it **way** easier to merge changes from upstream.
- the focus of this repo is to **port** marlin to the HC32F46x SoC. if you want to add new features, please consider creating a PR to the upstream marlin repo instead.
- keep your commits small and focused. this makes it easier to review and merge.
  - this also means that you should avoid changes to files not related to the HC32F46x SoC, like for example the vscode config files.
- if you make changes that require changes to the arduino core, create a PR in both repos and add links to each other.

# Disclaimer

my abilities to debug the firmware are currently extremely limited (i basically just compile, flash, and pray).
because of this, i cannot offer much support for the firmware, and the following is a bit more harsh than i'd normally do (sorry). So here goes:

this firmware comes without **any** support or gurantees.
if you brick your printer, you're on your own.

- issues opened demanding a bug to be fixed (without any intention of helping) will be closed and/or ignored.
- issues requesting new features will be closed. this is just a port of vanilla marlin.
- again, if you break your printer, you're on your own.

# Building

the firmware can be built using two main methods, which are both very similar to how you'd build Marlin normally.

please note that **ALL** methods described here require that [git](https://git-scm.com/) is installed (don't have to be logged in).

## AutoBuildMarlin

building the firmware using AutoBuildMarlin should be the same as with any other Marlin build.

1. When opening this project in VSCode, you'll be prompted to install the recommended extensions (PlatformIO and AutoBuildMarlin).
2. open `Marlin/Configuration.h` and `Marlin/Configuration_adv.h` and configure the firmware to your liking. for example configurations, see [shadow578/Marlin-Configurations-H32](https://github.com/shadow578/Marlin-Configurations-H32)
3. the Auto Build Marlin panel should now show the following details:
   - Firmware: Marlin bugfix-2.1.x
   - Board: AQUILA X2 H32
   - Pins: hc32f46x/pins_Aquila_X2.h
   - Architectures: HC32F46x
   - Environments: HC32F46x_AQUILA_X2
4. press the 'Build' Button on the Environment 'HC32F46x_AQUILA_X2' to build the firmware. After the build completes, the file explorer should open with the `firmware.bin` file already selected. If not, the file will be located in `.pio/build/HC32F46x_AQUILA_X2/`.

> Note: upload is not supported. you'll have to manually copy the `firmware.bin` file to the SD card and install it as described in the [Installation](#installation) section.

## PlatformIO

building the firmware using PlatformIO is quite similar to building with AutoBuildMarlin, as AutoBuildMarlin uses PlatformIO under the hood.

1. switch the PlatformIO environment to `HC32F46x_AQUILA_X2`.
2. open `Marlin/Configuration.h` and `Marlin/Configuration_adv.h` and configure the firmware to your liking. for example configurations, see [shadow578/Marlin-Configurations-H32](https://github.com/shadow578/Marlin-Configurations-H32)
3. build the firmware by pressing the 'Build' button in the bottom left corner of VSCode. After the build completes, the `firmware.bin` file should be located in `.pio/build/HC32F46x_AQUILA_X2/`.

> Note: upload is not supported. you'll have to manually copy the `firmware.bin` file to the SD card and install it as described in the [Installation](#installation) section.

# Installation

installing the firmware onto your printer is fairly straight forward

1. before doing anything, you might want to download the stock firmware from [Voxelab](https://www.voxelab3dp.com/download).
   - Ensure that both `firmware.bin` and `DWIN_SET` are present
2. build or download `firmware.bin` (printer firmware)
3. download the `DWIN_SET` that corresponds to the ui selected in `Configuration.h` (`DWIN_***` option).
   - downloads are available as part of the Ender-3 example configurations. see [Marlin/src/lcd/e3v2/README.md](./Marlin/src/lcd/e3v2/README.md) for details on where to find `DWIN_SET`.
   - ensure you download from the correct tag. the tag selected in github must match the `SHORT_BUILD_VERSION` specified in `Marlin/Version.h` (eg. `bugfix-2.1.x`)
4. format a SD Card (<= 16Gb) as FAT32
5. create a folder `firmware` in the root of the SD card and place `firmware.bin` into the folder
6. copy `DWIN_SET` directory into the root of the SD card
7. with your 3D-Printer powered down, insert the SD card into the Printers SD slot
8. power on your printer. You should now see a progress bar appear on the screen.
   - after the update finishes, the screen may appear garbled. this is normal.
9. power down your printer, then insert the SD card into the Displays SD slot.
   - you have to disassemble the screen for this.
10. power on your printer. the screen should be solid blue. wait for it to go solid orange.
11. power down your printer and remove the SD card.
12. the firmware is now installed.

if you don't like reading, you can watch [this video](https://www.youtube.com/watch?v=6afQUIR6Dmo) instead.
you'll have to exchange the `firmware.bin` and `DWIN_SET` for the ones mentioned here, but the process otherwise is the same.

## Support for other Printers

although this project is mainly aimed to work on the voxlab aquila X2 printer, it may also work on other 3D-printers with the HC32F46x (H32) SoC.

for other printers to work with this firmware, you'll have to (at least) ensure the following things match your printer:

- main configuration files (`Configuration.h` and `Configuration_adv.h`)
  - this one is fairly simple, you'll have to find and port the changes your printer vendor applied
- pin definitions (under `Marlin/src/pins/hc32f46x`) \*
  - again, you'll have to find the pin definition your printer vendor used and port it over. use the existing pin definitions as a reference.
- SoC flash size \*
  - for this, you can take a look at the SoC soldered to the mainboard. Look up the datasheet for the SoC and find the flash size.
  - once you have the flash size, set the config option `board_build.ld_args.flash_size` in `ini/h32.ini` to the flash size (eg. `256K` for 256Kb flash)
  - if you're unsure, you can just leave it at the default. this matches the 256kb variant of the hc32f46x series, which is the smallest size available
- app start address / bootloader entrypoint \*
  - see the following section for finding the address
  - once you found the address, set the config option `board_build.ld_args.flash_start` in `ini/h32.ini` to the address (eg. `0x0000C000`)

> \* if you change these and successfully build and run the firmware, please create a new environment for your printer and submit a PR. see the `[env:HC32F46x_AQUILA_X2]` environment in `ini/h32.ini` for an example.

### Finding the app start address

finding the app start address may be a bit tricky...
you'll have to find the address where the firmware entry point resides (eg. where the bootloader jumps to when starting to run the firmware)

**using a boot log**

with any luck, the printer may print the app start address during boot.
to find this, connect your printer using a serial cable and cause a soft-reset of the printer (by sending the `M997` gcode).

now, observe the output on the serial console. it may look something like this:

```
[...]
version 1.2
sdio init success!
Disk init
Tips ------ None Firmware file
GotoApp->addr=0xC000

start
[...]
```

the value you're looking for will pre printed _before_ the printer sends 'start' ('start' is sent by marlin itself).
in this case, the app start address is printed to be `0xC000`.

**using the firmware source code**

somewhere in the startup section, there should be a line similar to `SCB->VTOR = ((uint32_t) APP_START_ADDRESS & SCB_VTOR_TBLOFF_Msk);`.
if you find this line, the variable `APP_START_ADDRESS` contains the value you're looking for.

**using a linker script**

find the linker script file (a .ld file).
the file should contain a section like this:

```
MEMORY
{
    FLASH       (rx): ORIGIN = 0x0000C000, LENGTH = 512K
    OTP         (rx): ORIGIN = 0x03000C00, LENGTH = 1020
    RAM        (rwx): ORIGIN = 0x1FFF8000, LENGTH = 188K
    RET_RAM    (rwx): ORIGIN = 0x200F0000, LENGTH = 4K
}
```

in this case, the app start address is equal to the origin of the FLASH section, so 0x0000C000.
if there are multiple sections called FLASH\_\*\*\*, use the origin of the first one.

## Documentation on the HC32F46x SoC

documentation on the HC32F46x SoCs can be made available upon request.
available documents include:

- datasheet
- user manual (register overview, etc.)
- DDL provided by hdsc, including manual and usage examples
- programming tools and emulators

> Note: i'm not publishing these documents as i'm unsure on their license.

---

<!-- begin original README -->

<p align="center"><img src="buildroot/share/pixmaps/logo/marlin-outrun-nf-500.png" height="250" alt="MarlinFirmware's logo" /></p>

<h1 align="center">Marlin 3D Printer Firmware</h1>

<p align="center">
    <a href="/LICENSE"><img alt="GPL-V3.0 License" src="https://img.shields.io/github/license/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/graphs/contributors"><img alt="Contributors" src="https://img.shields.io/github/contributors/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/releases"><img alt="Last Release Date" src="https://img.shields.io/github/release-date/MarlinFirmware/Marlin"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/actions"><img alt="CI Status" src="https://github.com/MarlinFirmware/Marlin/actions/workflows/test-builds.yml/badge.svg"></a>
    <a href="https://github.com/sponsors/thinkyhead"><img alt="GitHub Sponsors" src="https://img.shields.io/github/sponsors/thinkyhead?color=db61a2"></a>
    <br />
    <a href="https://fosstodon.org/@marlinfirmware"><img alt="Follow MarlinFirmware on Mastodon" src="https://img.shields.io/mastodon/follow/109450200866020466?domain=https%3A%2F%2Ffosstodon.org&logoColor=%2300B&style=social"></a>
</p>

Additional documentation can be found at the [Marlin Home Page](https://marlinfw.org/).
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

## Marlin 2.1 Bugfix Branch

**Not for production use. Use with caution!**

Marlin 2.1 takes this popular RepRap firmware to the next level by adding support for much faster 32-bit and ARM-based boards while improving support for 8-bit AVR boards. Read about Marlin's decision to use a "Hardware Abstraction Layer" below.

This branch is for patches to the latest 2.1.x release version. Periodically this branch will form the basis for the next minor 2.1.x release.

Download earlier versions of Marlin on the [Releases page](https://github.com/MarlinFirmware/Marlin/releases).

## Example Configurations

Before you can build Marlin for your machine you'll need a configuration for your specific hardware. Upon request, your vendor will be happy to provide you with the complete source code and configurations for your machine, but you'll need to get updated configuration files if you want to install a newer version of Marlin. Fortunately, Marlin users have contributed dozens of tested configurations to get you started. Visit the [MarlinFirmware/Configurations](https://github.com/MarlinFirmware/Configurations) repository to find the right configuration for your hardware.

## Building Marlin 2.1

To build and upload Marlin you will use one of these tools:

- The free [Visual Studio Code](https://code.visualstudio.com/download) using the [Auto Build Marlin](https://marlinfw.org/docs/basics/auto_build_marlin.html) extension.
- The free [Arduino IDE](https://www.arduino.cc/en/main/software) : See [Building Marlin with Arduino](https://marlinfw.org/docs/basics/install_arduino.html)
- You can also use VSCode with devcontainer : See [Installing Marlin (VSCode devcontainer)](http://marlinfw.org/docs/basics/install_devcontainer_vscode.html).

Marlin is optimized to build with the **PlatformIO IDE** extension for **Visual Studio Code**. You can still build Marlin with **Arduino IDE**, and we hope to improve the Arduino build experience, but at this time PlatformIO is the better choice.

## Hardware Abstraction Layer (HAL)

Marlin includes an abstraction layer to provide a common API for all the platforms it targets. This allows Marlin code to address the details of motion and user interface tasks at the lowest and highest levels with no system overhead, tying all events directly to the hardware clock.

Every new HAL opens up a world of hardware. At this time we need HALs for RP2040 and the Duet3D family of boards. A HAL that wraps an RTOS is an interesting concept that could be explored. Did you know that Marlin includes a Simulator that can run on Windows, macOS, and Linux? Join the Discord to help move these sub-projects forward!

## 8-Bit AVR Boards

A core tenet of this project is to keep supporting 8-bit AVR boards while also maintaining a single codebase that applies equally to all machines. We want casual hobbyists to benefit from the community's innovations as much as possible just as much as those with fancier machines. Plus, those old AVR-based machines are often the best for your testing and feedback!

### Supported Platforms

| Platform                                                                                                                                                                                               | MCU             | Example Boards                                             |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | --------------- | ---------------------------------------------------------- |
| [Arduino AVR](https://www.arduino.cc/)                                                                                                                                                                 | ATmega          | RAMPS, Melzi, RAMBo                                        |
| [Teensy++ 2.0](https://www.microchip.com/en-us/product/AT90USB1286)                                                                                                                                    | AT90USB1286     | Printrboard                                                |
| [Arduino Due](https://www.arduino.cc/en/Guide/ArduinoDue)                                                                                                                                              | SAM3X8E         | RAMPS-FD, RADDS, RAMPS4DUE                                 |
| [ESP32](https://github.com/espressif/arduino-esp32)                                                                                                                                                    | ESP32           | FYSETC E4, E4d@BOX, MRR                                    |
| [LPC1768](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1700-cortex-m3/512-kb-flash-64-kb-sram-ethernet-usb-lqfp100-package:LPC1768FBD100) | ARM® Cortex-M3  | MKS SBASE, Re-ARM, Selena Compact                          |
| [LPC1769](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1700-cortex-m3/512-kb-flash-64-kb-sram-ethernet-usb-lqfp100-package:LPC1769FBD100) | ARM® Cortex-M3  | Smoothieboard, Azteeg X5 mini, TH3D EZBoard                |
| [STM32F103](https://www.st.com/en/microcontrollers-microprocessors/stm32f103.html)                                                                                                                     | ARM® Cortex-M3  | Malyan M200, GTM32 Pro, MKS Robin, BTT SKR Mini            |
| [STM32F401](https://www.st.com/en/microcontrollers-microprocessors/stm32f401.html)                                                                                                                     | ARM® Cortex-M4  | ARMED, Rumba32, SKR Pro, Lerdge, FYSETC S6, Artillery Ruby |
| [STM32F7x6](https://www.st.com/en/microcontrollers-microprocessors/stm32f7x6.html)                                                                                                                     | ARM® Cortex-M7  | The Borg, RemRam V1                                        |
| [STM32G0B1RET6](https://www.st.com/en/microcontrollers-microprocessors/stm32g0x1.html)                                                                                                                 | ARM® Cortex-M0+ | BigTreeTech SKR mini E3 V3.0                               |
| [STM32H743xIT6](https://www.st.com/en/microcontrollers-microprocessors/stm32h743-753.html)                                                                                                             | ARM® Cortex-M7  | BigTreeTech SKR V3.0, SKR EZ V3.0, SKR SE BX V2.0/V3.0     |
| [SAMD51P20A](https://www.adafruit.com/product/4064)                                                                                                                                                    | ARM® Cortex-M4  | Adafruit Grand Central M4                                  |
| [Teensy 3.5](https://www.pjrc.com/store/teensy35.html)                                                                                                                                                 | ARM® Cortex-M4  |
| [Teensy 3.6](https://www.pjrc.com/store/teensy36.html)                                                                                                                                                 | ARM® Cortex-M4  |
| [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)                                                                                                                                                 | ARM® Cortex-M7  |
| [Teensy 4.1](https://www.pjrc.com/store/teensy41.html)                                                                                                                                                 | ARM® Cortex-M7  |
| Linux Native                                                                                                                                                                                           | x86/ARM/etc.    | Raspberry Pi                                               |

## Submitting Patches

Proposed patches should be submitted as a Pull Request against the ([bugfix-2.1.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.1.x)) branch.

- This branch is for fixing bugs and integrating any new features for the duration of the Marlin 2.1.x life-cycle.
- Follow the [Coding Standards](https://marlinfw.org/docs/development/coding_standards.html) to gain points with the maintainers.
- Please submit Feature Requests and Bug Reports to the [Issue Queue](https://github.com/MarlinFirmware/Marlin/issues/new/choose). Support resources are also listed there.
- Whenever you add new features, be sure to add tests to `buildroot/tests` and then run your tests locally, if possible.
  - It's optional: Running all the tests on Windows might take a long time, and they will run anyway on GitHub.
  - If you're running the tests on Linux (or on WSL with the code on a Linux volume) the speed is much faster.
  - You can use `make tests-all-local` or `make tests-single-local TEST_TARGET=...`.
  - If you prefer Docker you can use `make tests-all-local-docker` or `make tests-all-local-docker TEST_TARGET=...`.

## Marlin Support

The Issue Queue is reserved for Bug Reports and Feature Requests. To get help with configuration and troubleshooting, please use the following resources:

- [Marlin Documentation](https://marlinfw.org) - Official Marlin documentation
- [Marlin Discord](https://discord.gg/n5NJ59y) - Discuss issues with Marlin users and developers
- Facebook Group ["Marlin Firmware"](https://www.facebook.com/groups/1049718498464482/)
- RepRap.org [Marlin Forum](https://forums.reprap.org/list.php?415)
- Facebook Group ["Marlin Firmware for 3D Printers"](https://www.facebook.com/groups/3Dtechtalk/)
- [Marlin Configuration](https://www.youtube.com/results?search_query=marlin+configuration) on YouTube

## Contributors

Marlin is constantly improving thanks to a huge number of contributors from all over the world bringing their specialties and talents. Huge thanks are due to [all the contributors](https://github.com/MarlinFirmware/Marlin/graphs/contributors) who regularly patch up bugs, help direct traffic, and basically keep Marlin from falling apart. Marlin's continued existence would not be possible without them.

## Administration

Regular users can open and close their own issues, but only the administrators can do project-related things like add labels, merge changes, set milestones, and kick trolls. The current Marlin admin team consists of:

<table align="center">
<tr><td>Project Maintainer</td></tr>
<tr><td>

🇺🇸  **Scott Lahteine**
      [@thinkyhead](https://github.com/thinkyhead)
      [<kbd>  Donate 💸  </kbd>](https://www.thinkyhead.com/donate-to-marlin)

</td><td>

🇺🇸  **Roxanne Neufeld**
      [@Roxy-3D](https://github.com/Roxy-3D)

🇺🇸  **Keith Bennett**
      [@thisiskeithb](https://github.com/thisiskeithb)
      [<kbd>  Donate 💸  </kbd>](https://github.com/sponsors/thisiskeithb)

🇺🇸  **Jason Smith**
      [@sjasonsmith](https://github.com/sjasonsmith)

</td><td>

🇧🇷  **Victor Oliveira**
      [@rhapsodyv](https://github.com/rhapsodyv)

🇬🇧  **Chris Pepper**
      [@p3p](https://github.com/p3p)

🇳🇿  **Peter Ellens**
      [@ellensp](https://github.com/ellensp)
      [<kbd>  Donate 💸  </kbd>](https://ko-fi.com/ellensp)

</td><td>

🇺🇸  **Bob Kuhn**
      [@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)

🇳🇱  **Erik van der Zalm**
      [@ErikZalm](https://github.com/ErikZalm)
      [<kbd>  Donate 💸  </kbd>](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)

</td></tr>
</table>

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
