# Marlin 3D Printer Firmware

[![Build Status](https://travis-ci.org/MarlinFirmware/Marlin.svg?branch=RCBugFix)](https://travis-ci.org/MarlinFirmware/Marlin)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224)

<img align="top" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

Additional documentation can be found at the [Marlin Home Page](http://marlinfw.org/).
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

## Marlin 2.0 Bugfix Branch

__Not for production use. Use with caution!__

This branch is used to accumulate patches to the latest 2.0.x release version. Periodically this branch will form the basis for the next minor 2.0.x release.

Download earlier versions of Marlin on the [Releases page](https://github.com/MarlinFirmware/Marlin/releases).

## Recent Changes
- Add a Hardware Abstraction Layer (HAL) supporting current AVR platforms
- Planning a HAL for Re:Arm
- Planning a HAL for STM32

## Submitting Patches

Proposed patches should be submitted as a Pull Request against this branch ([bugfix-2.0.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x)).

- This branch is for fixing bugs and integrating any new features for the duration of the Marlin 2.0.x life-cycle.
- Follow the [Coding Standards](http://marlinfw.org/docs/development/coding_standards.html) to gain points with the maintainers.
- Please submit your questions and concerns to the [Issue Queue](https://github.com/MarlinFirmware/Marlin/issues).

### [RepRap.org Wiki Page](http://reprap.org/wiki/Marlin)

## Credits

The current Marlin dev team consists of:
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)] - English
 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)] - English
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)] - English
 - Chris Pepper [[@p3p](https://github.com/p3p)] - English
 - João Brazio [[@jbrazio](https://github.com/jbrazio)] - Portuguese, English

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
