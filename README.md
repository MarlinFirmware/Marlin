# Marlin 3D Printer Firmware

[![Build Status](https://travis-ci.org/MarlinFirmware/Marlin.svg?branch=RCBugFix)](https://travis-ci.org/MarlinFirmware/Marlin)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224)

<img align="top" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

Additional documentation can be found at the [Marlin Home Page](http://marlinfw.org/).
Please test this firmware and inform us if it misbehaves in any way, volunteers are standing by!

## Bugfix Branch

__Not for production use. Use with caution!__

This branch is used to accumulate patches to the latest 1.1.x release version. Periodically this branch will form the basis for the next minor 1.1.x release.

Download earlier versions of Marlin on the [Releases page](https://github.com/MarlinFirmware/Marlin/releases). (The latest tagged release of Marlin is version 1.1.0.)

## Recent Changes
- 1.1.0 - 4 May 2017
  - See the [1.1.0 Release Notes](https://github.com/MarlinFirmware/Marlin/releases/tag/1.1.0) for a full list of changes.

- RC8 - 6 Dec 2016
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
  - Add support for `SINGLENOZZLE`, `MIXING_EXTRUDER`, `SWITCHING_NOZZLE`, and `SWITCHING_EXTRUDER`
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

Proposed patches should be submitted as a Pull Request against this branch ([bugfix-1.1.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-1.1.x)).

- This branch is for fixing bugs and integrating any new features for the duration of the Marlin 1.1.x life-cycle. We've opted for a simplified branch structure while we work on the maintainability and encapsulation of code modules. Version 1.2 and beyond should improve on separation of bug fixes and cutting-edge development.
- Follow the proper coding style to gain points with the maintainers. See our [Coding Standards](http://marlinfw.org/docs/development/coding_standards.html) page for more information.
- Please submit your questions and concerns to the [Issue Queue](https://github.com/MarlinFirmware/Marlin/issues). The "naive" question is often the one we forget to ask.

### [RepRap.org Wiki Page](http://reprap.org/wiki/Marlin)

## Credits

The current Marlin dev team consists of:
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)] - English
 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)] - English
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)] - English
 - Andreas Hardtung [[@AnHardt](https://github.com/AnHardt)] - Deutsch, English
 - Nico Tonnhofer [[@Wurstnase](https://github.com/Wurstnase)] - Deutsch, English
 - Jochen Groppe [[@CONSULitAS](https://github.com/CONSULitAS)] - Deutsch, English
 - João Brazio [[@jbrazio](https://github.com/jbrazio)] - Portuguese, English
 - Bo Hermannsen [[@boelle](https://github.com/boelle)] - Danish, English
 - Bob Cousins [[@bobc](https://github.com/bobc)] - English
 - [[@maverikou](https://github.com/maverikou)]
 - Chris Palmer [[@nophead](https://github.com/nophead)]
 - [[@paclema](https://github.com/paclema)]
 - Erik van der Zalm [[@ErikZalm](https://github.com/ErikZalm)]
 - David Braam [[@daid](https://github.com/daid)]
 - Bernhard Kubicek [[@bkubicek](https://github.com/bkubicek)]

More features have been added by:
 - Alberto Cotronei [[@MagoKimbra](https://github.com/MagoKimbra)] - English, Italian
 - Thomas Moore [[@tcm0116](https://github.com/tcm0116)]
 - Ernesto Martinez [[@emartinez167](https://github.com/emartinez167)]
 - Petr Zahradnik [[@clexpert](https://github.com/clexpert)]
 - Kai [[@Kaibob2](https://github.com/Kaibob2)]
 - Edward Patel [[@epatel](https://github.com/epatel)]
 - F. Malpartida [[@fmalpartida](https://github.com/fmalpartida)] - English, Spanish
 - [[@esenapaj](https://github.com/esenapaj)] - English, Japanese
 - [[@benlye](https://github.com/benlye)]
 - [[@Tannoo](https://github.com/Tannoo)]
 - [[@teemuatlut](https://github.com/teemuatlut)]
 - [[@bgort](https://github.com/bgort)]
 - [[@LVD-AC](https://github.com/LVD-AC)]
 - [[@paulusjacobus](https://github.com/paulusjacobus)]
 - ...and many others

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
