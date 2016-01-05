# Marlin 3D Printer Firmware
<img align="top" width=175 src="Documentation/Logo/Marlin%20Logo%20GitHub.png" />
 Additional documentation can be found in [our wiki](https://github.com/MarlinFirmware/Marlin/wiki/Main-Page).

## Release Candidate -- Marlin 1.1.0-RC3 - 01 December 2015

__Not for production use – use with caution!__

Previously tagged versions of Marlin are not recommended. However, the latest patches to the Marlin 1.0 series can be found in the [1.0.x](https://github.com/MarlinFirmware/Marlin/tree/1.0.x) branch.

[This branch, "RC"](https://github.com/MarlinFirmware/Marlin/tree/RC), is our current pre-release candidate.

Future development takes place in the [MarlinDev repository](https://github.com/MarlinFirmware/MarlinDev/).

## Recent Changes
RC3 - 01 Dec 2015
      A number of language sensitive strings have been revised
      Formatting of the LCD display has been improved to handle negative coordinates better
      Various compiler-related issues have been corrected

RC2 - 29 Sep 2015
      File styling reverted
      LCD update frequency reduced

RC1 - 19 Sep 2015
      Published for testing

## Submitting Patches
Proposed patches should be submitted as a Pull Request against the [RCBugFix](https://github.com/MarlinFirmware/Marlin/tree/RCBugFix) branch
Note: Do not propose added features. Patches to the Release Candidate are limited to corrections of coding errors for the functions that have been implemented.
Please follow the proper style. Pull requests containing styling errors will have to be reworked.



## Current Status: Testing

Please test this firmware and inform us of any issues where it fails to perform in a manner consistent with the designed functionality.

[![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224)
[![Travis Build Status](https://travis-ci.org/MarlinFirmware/MarlinDev.svg)](https://travis-ci.org/MarlinFirmware/MarlinDev)

##### [RepRap.org Wiki Page](http://reprap.org/wiki/Marlin)

## Contact

__Google Hangout:__ <a href="https://plus.google.com/hangouts/_/gxn3wrea5gdhoo223yimsiforia" target="_blank">. Hangout</a> We have a hangout every 2 weeks. Search the issue list for "Hangout" and you will see what time and date the next event is going on.

## Credits

The current Marlin dev team consists of:

 - Andreas Hardtung [@AnHardt] - Deutsch, English
 - Scott Lahteine [@thinkyhead] - English
 - [@Wurstnase] - Deutsch, English
 - F. Malpartida [@fmalpartida] - English, Spanish
 - [@CONSULitAS] - Deutsch, English
 - [@maverikou]
 - Chris Palmer [@nophead]
 - [@paclema]
 - Edward Patel [@epatel] - Swedish, English
 - Erik van der Zalm [@ErikZalm]
 - David Braam [@daid]
 - Bernhard Kubicek [@bkubicek]
 - Richard Wackerbarth [@Wackerbarth] - English
 - Roxanne Neufeld [@Roxy-3DPrintBoard] - English

More features have been added by:
  - Alberto Cotronei [@MagoKimbra]
  - Lampmaker,
  - Bradley Feldman,
  - and others...

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.

[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)
