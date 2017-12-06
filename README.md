# Marlin 3D Printer Firmware (AON Fork)

## Branches

* `aon-m1` - to be used on the OG AON-M1 printer
* `aon-m2` - to be used on the new AON-M2

*Please do not commit directly to either of these branches. All changes must be reviewed and approved.*

Additional documentation can be found at the [Marlin Home Page](http://marlinfw.org/).

### Current HALs

  name|processor|speed|flash|sram|logic|fpu
  ----|---------|-----|-----|----|-----|---
  [Arduino AVR](https://www.arduino.cc/)|ATmega, ATTiny, etc.|16-20MHz|64-256k|2-8k|5V|no
  [Teensy++ 2.0](http://www.microchip.com/wwwproducts/en/AT90USB1286)|[AT90USB1286](http://www.microchip.com/wwwproducts/en/AT90USB1286)|16MHz|128k|8k|5V|no
  [Due](https://www.arduino.cc/en/Guide/ArduinoDue), [RAMPS-FD](http://www.reprap.org/wiki/RAMPS-FD), etc.|[SAM3X8E ARM-Cortex M3](http://www.microchip.com/wwwproducts/en/ATsam3x8e)|84MHz|512k|64+32k|3.3V|no
  [Re-ARM](https://www.kickstarter.com/projects/1245051645/re-arm-for-ramps-simple-32-bit-upgrade)|[LPC1768 ARM-Cortex M3](http://www.nxp.com/products/microcontrollers-and-processors/arm-based-processors-and-mcus/lpc-cortex-m-mcus/lpc1700-cortex-m3/512kb-flash-64kb-sram-ethernet-usb-lqfp100-package:LPC1768FBD100)|100MHz|512k|32+16+16k|3.3-5V|no
  [MKS SBASE](http://forums.reprap.org/read.php?13,499322)|LPC1768 ARM-Cortex M3|100MHz|512k|32+16+16k|3.3-5V|no
  [Azteeg X5 GT](https://www.panucatt.com/azteeg_X5_GT_reprap_3d_printer_controller_p/ax5gt.htm)|LPC1769 ARM-Cortex M3|120MHz|512k|32+16+16k|3.3-5V|no
  [Teensy 3.5](https://www.pjrc.com/store/teensy35.html)|ARM-Cortex M4|120MHz|512k|192k|3.3-5V|yes
  [Teensy 3.6](https://www.pjrc.com/store/teensy36.html)|ARM-Cortex M4|180MHz|1M|256k|3.3V|yes

### HALs in Development

  name|processor|speed|flash|sram|logic|fpu
  ----|---------|-----|-----|----|-----|---
  [STEVAL-3DP001V1](http://www.st.com/en/evaluation-tools/steval-3dp001v1.html)|[STM32F401VE Arm-Cortex M4](http://www.st.com/en/microcontrollers/stm32f401ve.html)|84MHz|512k|64+32k|3.3-5V|yes
  [Smoothieboard](http://reprap.org/wiki/Smoothieboard)|LPC1769 ARM-Cortex M3|120MHz|512k|64k|3.3-5V|no

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
