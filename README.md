# Marlin 3D Printer Firmware

[![Build Status](https://travis-ci.org/MarlinFirmware/Marlin.svg?branch=RCBugFix)](https://travis-ci.org/MarlinFirmware/Marlin)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224)

<img align="top" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

Additional documentation can be found at [The Marlin Documentation Project](https://www.marlinfw.org/).
Please test this firmware and inform us if it misbehaves in any way, volunteers are standing by!

## Release Candidate -- Marlin 1.1.0-RCBugFix - 6 Dec 2016

__Not for production use – use with caution!__

You can download earlier versions of Marlin on the [Releases page](https://github.com/MarlinFirmware/Marlin/releases). (The latest "stable" release of Marlin is 1.0.2-1.)

The latest Release Candidate lives in the ["RC" branch](https://github.com/MarlinFirmware/Marlin/tree/RC). Bugs that we find in the current Release Candidate are patched in the ["RCBugFix" branch](https://github.com/MarlinFirmware/Marlin/tree/RCBugFix), so during beta testing this is where you can always find the latest code on its way towards release.

## Recent Changes
- RCBugFix
  - Fixed broken MBL
  - M600 heater timeout option

- RC8 - 06 Dec 2016
  - Major performance improvement for Graphical LCDs
  - Simplified probe configuration
  - Enable Auto Bed Leveling by type
  - Reduce serial communication errors
  - Make Bilinear (Mesh) Grid Leveling available for non-delta
  - Support for Trinamic TMC2130 SilentStepStick SPI-based drivers
  - Add `M211` to Enable/Disable Software Endstops
  - Add `M355` to turn the case light on/off and set brightness
  - Improved I2C class with full master/slave support
  - Add `G38.2` `G38.3` command option for CNC style probing
  - Add `MINIMUM_STEPPER_PULSE` option to adjust step pulse duration
  - Add `R` parameter support for `G2`/`G3`
  - Add `M43` optional command (`PINS_DEBUGGING`)
  - Remove SCARA axis scaling
  - Improved sanity checking of configuration
  - Improved support for PlatformIO and command-line build
  - Usable `DELTA_CALIBRATION_MENU`
  - Support for Printrbot Rev.F
  - New and updated languages

- RC7 - 26 Jul 2016
  - Add Print Job Timer and Print Counter (`PRINTCOUNTER`)
  - New `M600` Filament Change (`FILAMENT_CHANGE_FEATURE`)
  - New `G12` Nozzle Clean (`NOZZLE_CLEAN_FEATURE`)
  - New `G27` Nozzle Park (`NOZZLE_PARK_FEATURE`)
  - Add support for `COREYZ`
  - Add a new Advance Extrusion algorithm (`LIN_ADVANCE`)
  - Add support for inches, Fahrenheit, Kelvin units (`INCH_MODE_SUPPORT`, `TEMPERATURE_UNITS_SUPPORT`)
  - Better handling of `G92` shifting of the coordinate space
  - Add Greek and Croatian languages
  - Improve the Manual (Mesh) Bed Leveling user interface
  - Add support for more boards, controllers, and probes:
    - Vellemann K8400 (`BOARD_K8400`)
    - RigidBot V2 (`BOARD_RIGIDBOARD_V2`)
    - Cartesio UI (`BOARD_CNCONTROLS_12`)
    - BLTouch probe sensor (`BLTOUCH`)
    - Viki 2 with RAMPS and MKS boards
  - Improve support for `DELTA` and other kinematics
  - Improve thermal management, add `WATCH_BED_TEMP_PERIOD`
  - Better handling of toolchange, multiple tools
  - Add support for two X steppers `X_DUAL_STEPPER_DRIVERS`
  - Add support for `SINGLENOZZLE`, `MIXING_EXTRUDER`, and `SWITCHING_EXTRUDER`
  - Simplified probe configuration, allow usage without bed leveling
  - And much more… See the [1.1.0-RC7 Change Log](https://github.com/MarlinFirmware/Marlin/releases/tag/1.1.0-RC7) for the complete list of changes.

- RC6 - 24 Apr 2016
  - Marlin now requires Arduino version 1.6.0 or later
  - Completed support for CoreXY / CoreXZ
  - See the [1.1.0-RC6 Change Log](https://github.com/MarlinFirmware/Marlin/releases/tag/1.1.0-RC6) for all the changes.

- RC5 - 01 Apr 2016
  - Warn if compiling with older versions (<1.50) of Arduino
  - Fix various LCD menu issues
  - Add formal support for MKSv1.3 and Sainsmart (RAMPS variants)
  - Fix bugs in M104, M109, and M190
  - Fix broken M404 command
  - Fix issues with M23 and "Start SD Print"
  - More output for M111
  - Rename FILAMENT_SENSOR to FILAMENT_WIDTH_SENSOR
  - Fix SD card bugs
  - and a lot more
  - See the [1.1.0-RC5 Change Log](https://github.com/MarlinFirmware/Marlin/releases/tag/1.1.0-RC5) for more!

- RC4 - 24 Mar 2016
  - Many lingering bugs and nagging issues addressed
  - Improvements to LCD menus, CoreXY/CoreXZ, Delta, Bed Leveling, and more…

- RC3 - 01 Dec 2015
  - A number of language sensitive strings have been revised
  - Formatting of the LCD display has been improved to handle negative coordinates better
  - Various compiler-related issues have been corrected

- RC2 - 29 Sep 2015
  - File styling reverted
  - LCD update frequency reduced

- RC1 - 19 Sep 2015
  - Published for testing

## Submitting Patches
Proposed patches should be submitted as a Pull Request against the [RCBugFix](https://github.com/MarlinFirmware/Marlin/tree/RCBugFix) branch.

- Don't submit new feature proposals. The RCBugFix branch is for fixing bugs in existing features.
- Do submit questions and concerns. The "naive" question is often the one we forget to ask.
- Follow the proper coding style. Pull requests with styling errors will be delayed. See our [Coding Standards](https://github.com/MarlinFirmware/Marlin/wiki/DNE-Coding-Standards) page for more information.

### [RepRap.org Wiki Page](http://reprap.org/wiki/Marlin)

## Credits

The current Marlin dev team consists of:

 - Scott Lahteine [@thinkyhead] - English
 - [@Wurstnase] - Deutsch, English
 - F. Malpartida [@fmalpartida] - English, Spanish
 - Jochen Groppe [@CONSULitAS] - Deutsch, English
 - [@maverikou]
 - Chris Palmer [@nophead]
 - [@paclema]
 - Edward Patel [@epatel] - Swedish, English
 - Erik van der Zalm [@ErikZalm]
 - David Braam [@daid]
 - Bernhard Kubicek [@bkubicek]
 - Roxanne Neufeld [@Roxy-3DPrintBoard] - English

More features have been added by:
  - Alberto Cotronei [@MagoKimbra]
  - Lampmaker,
  - Bradley Feldman,
  - and others...

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
