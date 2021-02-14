# Marlin Firmware for the Creality Ender 3 v2 3D Printer 

![GitHub](https://img.shields.io/github/license/mriscoc/Marlin_Ender3v2.svg)
![GitHub contributors](https://img.shields.io/github/contributors/mriscoc/Marlin_Ender3v2.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/mriscoc/Marlin_Ender3v2.svg)
[![Build Status](https://github.com/mriscoc/Marlin_Ender3v2/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/mriscoc/Marlin_Ender3v2/actions)

<img align="right" width=200 src="buildroot/share/pixmaps/Ender-3V2.jpg" />

Additional documentation can be found at the [Marlin Home Page](https://marlinfw.org/).
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

__Not for production use. Use with caution!__

This fork is based on the bugfix-2.0.x branch of Marlin firmware.

## Building Marlin 2.0

To build Marlin 2.0 you'll need [Building Marlin with PlatformIO for ReArm](https://marlinfw.org/docs/basics/install_rearm.html) for the STM32F103RET6 Creality 32-bit boards.

## Download a compilated version of this firmware

Released versions of this firmware can be found on the [Releases page](https://github.com/mriscoc/Marlin_Ender3v2/releases).

## How to install the firmware

1. Insert a blank and FAT32 formatted µSD in your PC/Mac
2. Copy the .bin file to the root of the card
3. Ensure that the name of the file was not previously used to update the printer.
   If necessary, rename the file.
4. Turn off the printer, disconnect any USB cable and insert the µSD card.
5. Turn on the printer, the upgrade firmware process will start automatically.
6. The display will be blank until the upgrade is finished (about of 15 seconds).
7. If you are upgrading from the original firmware or other fork, please restore defaults.
8. Make the PID adjust and put the other calibration parameters
   
If your display remains blank try with other µSD card, format it to FAT32 and put in a renamed .bin file.
   
## Credits

This fork of Marlin firmware is manteined by Miguel Risco-Castillo [[@mriscoc](https://github.com/mriscoc)] - Peru

The current Marlin dev team consists of:

 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)] - USA &nbsp; [Donate](https://www.thinkyhead.com/donate-to-marlin) / Flattr: [![Flattr Scott](https://api.flattr.com/button/flattr-badge-small.png)](https://flattr.com/submit/auto?user_id=thinkyhead&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)] - USA
 - Chris Pepper [[@p3p](https://github.com/p3p)] - UK
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)] - USA
 - João Brazio [[@jbrazio](https://github.com/jbrazio)] - Portugal
 - Erik van der Zalm [[@ErikZalm](https://github.com/ErikZalm)] - Netherlands &nbsp; [![Flattr Erik](https://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.

THIS FIRMWARE AND ALL OTHER FILES IN THE DOWNLOAD ARE PROVIDED FREE OF CHARGE WITH NO WARRANTY OR GUARANTEE. SUPPORT IS NOT INCLUDED JUST BECAUSE YOU DOWNLOADED THE FIRMWARE.

WE ARE NOT LIABLE FOR ANY DAMAGE TO YOUR PRINTER, PERSON, OR ANY OTHER PROPERTY DUE TO USE OF THIS FIRMWARE. IF YOU DO NOT AGREE TO THESE TERMS THEN DO NOT USE THE FIRMWARE.
