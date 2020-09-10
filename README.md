# Unified 2.0 Firmware

Unified Firmware 2.0 is the latest easy to use firmware for select 3D printers and boards from TH3D Studio. Both 8 bit and 32 bit based machines/boards are supported thanks to being built on Marlin 2.0.

This firmware focuses on printers with their **stock** control boards and select aftermarket boards that we carry. If you have a 3rd party or other board installed in your machine it is recommended to use ["Vanilla" Marlin](https://marlinfw.org) on your machine.

## Building Unified 2.0 Firmware

To build Unified 2.0 Firmware will need to use PlatformIO and VSCode. Detailed build and install instructions are posted at:

  - [Setting up VSCode with PlatformIO](http://vscode.th3dstudio.com/)
  - Video Guide (Coming Soon)

## Supported Printers & Boards

**Creality Printers**

- Ender 3 V2
	- V4.2.2 Board
- Ender 3
	- V4.2.2 Board
	- [EZBoard Lite](https://ezboard.th3dstudio.com)
- Ender 3 Pro
	- V4.2.2 Board
	- [EZBoard Lite](https://ezboard.th3dstudio.com)
- Ender 5
	- V4.2.2 Board
	- [EZBoard Lite](https://ezboard.th3dstudio.com)
- Ender 5 Pro
	- V4.2.2 Board
	- [EZBoard Lite](https://ezboard.th3dstudio.com)

## Supported TH3D Upgrades

- [EZABL Pro Auto Bed Leveling Kit](https://ezabl.th3dstudio.com)

## Folder Structure

- **Firmware**
	- Main source code for the Unified 2 firmware. Copy your printer config files from the `Printer Configuration Files` folder
	- This is the folder you will open with VSCode after copying in your printer config files
- **Printer Configuration Files**
	- Files for each printer configuration that is supported by this firmware
- **STL Files**
	- Contains files for EZABL mounts, solid bed mounts, and bed level test files
- **TH3D Installation Guides**
	- Installation guides and accompanying files for TH3D products like the EZABL and EZOut filament sensor kits

## Technical Support

For best results getting help with configuration and troubleshooting, please use the following resources:

- [TH3D Forum](https://forum.th3dstudio.com)
- [TH3D Discord](http://Discord.TH3DStudio.com)
- [TH3D Help Center](https://support.th3dstudio.com)
- [3D Printer Support Facebook Group](https://www.facebook.com/groups/c3dforum/)
- [TH3D Customer Facebook Group](http://fbgroup.th3dstudio.com/)

## Credits

TH3D Unified 2.0 Firmware is built on top of Marlin Firmware. Please donate to their dev team if you wish to make a donation. This project is done by TH3D Studio LLC to provide the 3D printing community with easy to use firmware and also enable ease of use with our products and other products on the market. Profits from our store allow our technical team to make projects like this available for free to the community.

The current Marlin dev team consists of:

 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)] - USA &nbsp; [Donate](http://www.thinkyhead.com/donate-to-marlin) / Flattr: [![Flattr Scott](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=thinkyhead&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)] - USA
 - Chris Pepper [[@p3p](https://github.com/p3p)] - UK
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)] - USA
 - João Brazio [[@jbrazio](https://github.com/jbrazio)] - Portugal
 - Erik van der Zalm [[@ErikZalm](https://github.com/ErikZalm)] - Netherlands &nbsp; [![Flattr Erik](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)

## Firmware License

Marlin and Unified 2.0 Firmware is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.

## STL File License

Some STL files are licensed to only TH3D customers and not permitted for re-distrubution by other individuals or companies. License files are accompanied in the ZIP file/folder for STL files that have a separate license.