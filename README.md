# Marlin 3D Printer Firmware
<img align="right" src="../../raw/1.1.x/buildroot/share/pixmaps/logo/marlin-250.png" />

Marlin is the world's most popular open source firmware for Replicating Rapid Prototyper (RepRap) machines, commonly referred to as "3D printers." Marlin Firmware is highly efficient, running even on modest 16MHz embedded AVR processors. While Marlin 1.1 only supports ATmega AVR (Arduino, etc.) and AT90USB (Teensy++ 2.0), [Marlin 2.0](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x) also adds support for several ARM processors, including the SAM3X8E (Arduino Due), NXP LPC1768/LPC1769 ARM Cortex-M3 (Re-Arm, MKS SBASE, Smoothieboard), and ARM Cortex-M4 (Teensy 3.5/3.6, STM32F1/4/7).

A monumental amount of talent and effort goes into Marlin production, and thanks are due to many volunteers around the world. We work closely with the community, contributors, vendors, host and library developers, etc. to improve the quality, configurability, and compatibility of Marlin Firmware with a [huge variety](http://marlinfw.org/docs/configuration/configuration.html#motherboard) of boards. For the final 1.1 release we focused on code quality, performance, stability, and overall user experience. Several new features were added, many of which require no extra hardware.

## Documentation

- Visit [marlinfw.org](http://marlinfw.org/) for complete documentation on [configuration](http://marlinfw.org/docs/configuration/configuration.html), [installation](http://marlinfw.org/docs/basics/install.html), [features](http://marlinfw.org/meta/features/), and the many [G-codes](http://marlinfw.org/meta/gcode/) that Marlin supports. We will continue to expand the site to include in-depth articles, tutorials, and how-to videos on all of Marlin's features.
- See the [Releases](https://github.com/MarlinFirmware/Marlin/releases) page for Release Notes on all current and previous versions of Marlin.
- Check out the [RepRap.org Marlin Page](http://reprap.org/wiki/Marlin) for an overview of Marlin and its role in the RepRap project.

## Marlin 1.1.x

The 1.1.x branch is home to all tagged releases of Marlin 1.1.

Marlin 1.1.9 is the final release of the AVR-only flat version of Marlin Firmware, so there will be no further 1.1.x releases. However [`bugfix-1.1.x`](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x) will continue to receive patches for critical bugs, so be sure to test it (or [`bugfix-2.0.x`](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x)) before reporting any bugs you find in 1.1.9.

## Marlin 2.0.x

[Marlin 2.0](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x) is the future, featuring a much-improved hierarchical file structure and full [32-bit support](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x) via a Hardware Access Layer (HAL). Marlin 2.0 continues to work with [Arduino IDE](https://www.arduino.cc/en/Main/Software) for the platforms it supports, and the excellent [PlatformIO IDE](https://platformio.org/platformio-ide) is recommended for the next generation of ARM-based boards. If you're looking for the very best that Marlin has to offer and aren't bothered by a few rough edges, give version 2.0 a try!

## Contributing to Marlin

Click on the [Issue Queue](https://github.com/MarlinFirmware/Marlin/issues) and [Pull Requests](https://github.com/MarlinFirmware/Marlin/pulls) links above at any time to see what we're currently working on.

To submit patches and new features for Marlin 2.0 check out the [bugfix-2.0.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x) branch, add your commits, and submit a Pull Request back to the `bugfix-2.0.x` branch. Once 2.0.x has been certified for a critical mass of common 32-bit boards, it will become the next major release and will be the basis for all future major and minor releases.

Note that our "bugfix" branches always contain the latest patches and new code. These patches may not be widely tested. As always, when using "nightly" builds of Marlin, proceed with full caution.

## Marlin Resources

- [Marlin Home Page](http://marlinfw.org/) - The Marlin Documentation Project. [Help us](https://github.com/MarlinFirmware/MarlinDocumentation) expand this site!
- [@MarlinFirmware](https://twitter.com/MarlinFirmware) on Twitter - Follow for news, release alerts, and tips & tricks. (Maintained by [@thinkyhead](https://github.com/thinkyhead).)

## Marlin User Support

Looking for help? Our GitHub Issue Queue is only for development-related issues, feature requests, and bug reports. But there are several places where you can get help from experienced users:

- [RepRap.org Marlin Forum](http://forums.reprap.org/list.php?415)
- ["Marlin Firmware" Facebook Group](https://www.facebook.com/groups/1049718498464482/)
- [Tom's 3D Forums](https://discuss.toms3d.org/)
- [Marlin on Discord](https://discord.gg/n5NJ59y)

## Credits

Marlin Admins:
 - Erik van der Zalm [[@ErikZalm](https://github.com/ErikZalm)]
 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)]
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)]
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)]

Notable contributors:
 - Alberto Cotronei [[@MagoKimbra](https://github.com/MagoKimbra)]
 - Andreas Hardtung [[@AnHardt](https://github.com/AnHardt)]
 - Bernhard Kubicek [[@bkubicek](https://github.com/bkubicek)]
 - Bob Cousins [[@bobc](https://github.com/bobc)]
 - Chris Palmer [[@nophead](https://github.com/nophead)]
 - Chris Pepper [[@p3p](https://github.com/p3p)]
 - David Braam [[@daid](https://github.com/daid)]
 - Éduardo Tagle [[@ejtagle](https://github.com/ejtagle)]
 - Edward Patel [[@epatel](https://github.com/epatel)]
 - Ernesto Martinez [[@emartinez167](https://github.com/emartinez167)]
 - F. Malpartida [[@fmalpartida](https://github.com/fmalpartida)]
 - Giuliano Zaro [[@GMagician](https://github.com/GMagician)]
 - Jochen Groppe [[@CONSULitAS](https://github.com/CONSULitAS)]
 - João Brazio [[@jbrazio](https://github.com/jbrazio)]
 - Kai [[@Kaibob2](https://github.com/Kaibob2)]
 - Luc Van Daele[[@LVD-AC](https://github.com/LVD-AC)]
 - Nico Tonnhofer [[@Wurstnase](https://github.com/Wurstnase)]
 - Petr Zahradnik [[@clexpert](https://github.com/clexpert)]
 - Thomas Moore [[@tcm0116](https://github.com/tcm0116)]
 - [[@alexxy](https://github.com/alexxy)]
 - [[@android444](https://github.com/android444)]
 - [[@benlye](https://github.com/benlye)]
 - [[@bgort](https://github.com/bgort)]
 - [[@Grogyan](https://github.com/Grogyan)]
 - [[@marcio-ao](https://github.com/marcio-ao)]
 - [[@maverikou](https://github.com/maverikou)]
 - [[@oysteinkrog](https://github.com/oysteinkrog)]
 - [[@p3p](https://github.com/p3p)]
 - [[@paclema](https://github.com/paclema)]
 - [[@paulusjacobus](https://github.com/paulusjacobus)]
 - [[@psavva](https://github.com/psavva)]
 - [[@Tannoo](https://github.com/Tannoo)]
 - [[@teemuatlut](https://github.com/teemuatlut)]
 - ...and you!

## License

Marlin is published under the [GPL license](https://github.com/COPYING.md) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.

---

<!-- [![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224) -->
- [![Travis Build Status](https://travis-ci.org/MarlinFirmware/Marlin.svg)](https://travis-ci.org/MarlinFirmware/Marlin)
- [![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)
