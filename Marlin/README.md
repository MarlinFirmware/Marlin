# Marlin Firmware adapted for the FLSun HiSpeed v1


## Marlin 2.0.7<img align="right" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />


Marlin 2.0 takes this popular RepRap firmware to the next level by adding support for much faster 32-bit and ARM-based boards while improving support for 8-bit AVR boards. Read about Marlin's decision to use a "Hardware Abstraction Layer" below.

Download earlier versions of QQS-PRO Firmware on the [Releases page](https://github.com/Foxies-CSTL/Marlin2.0.X-FLSun_QQ-S/releases).

## Building Marlin 2.0

To build Marlin 2.0 you'll need [PlatformIO](http://docs.platformio.org/en/latest/ide.html#platformio-ide). Detailed build and install instructions are posted at:

 
  - [Installing Marlin (VSCode)](http://marlinfw.org/docs/basics/install_platformio_vscode.html).

### Supported Platforms

  Platform|MCU| Board
  --------|---|-------
  [FLSUN HiSpeed](https://FLSun.com.cn)|ARM Cortex-M3 / STM32F103VET6| FLSUN HiSpeed v1 
  [MKS Robin TFT v2](https://github.com/makerbase-mks/MKS-TFT)|ILI9341| TFT 2.8
  
### Features of the Preset Configuration

  Features|Active|Value
  --------|------|-----
  Fast Config Switch QQS-PRO|True|-
  UI Type|True|Color Marlin
  TFT Color Selection|True|Classic,Color
  EEPROM|True|-
  Recovery Support|True|-
  Arc Support|True|-
  Classic Jerk|True|10
  LinearAdvance|True|0
  Bezier curve acceleration|True|-
  Junction Deviation|False|0.013
  Unified Bed Leveling|True|-
  Filament sensor|True|-
  Recovery PowerFailed|True|-
  TMC 220x Soft Serial|UART|Ready
  TMC 2209 Hard Serial|UART|Ready
  Neopixel|-|Ready


  Confs|QQS_Stock(S)|QQS_TMC(8)|QQS_UART(U8)|QQS_SPE
  ----|----|----|----|----
  X|A4988|TMC2208 Standalone|TMC2208|TMC2208
  Y|A4988|TMC2208 Standalone|TMC2208|TMC2208
  Z|A4988|TMC2208 Standalone|TMC2208|TMC2208
  E|A4988|TMC2208 Standalone|TMC2208|A4988
  Wifi|True|Ready|False|False
  NeoPixel|False|Ready|False|True

## Changelog

Version|Changes & Fixes
-------|-------
1.1
  -|Major Update with Last Marlin(20201001)
1.0.9
  -|Add support different E0 driver
  -|Add NeoPixel support
  -|Commented error Neopixel Sanity Check
  -|fix Add Color Marlin (nanolib)
1.0.8
  -|Add Multi option (QQS_config)
  -|Add Adv. Preset 
  -|Add Preset PID HotEnd
  -|Add Preset PID Bed
1.0.7
  -|TMC mode UART
  -|Add Adv. Preset Custom PID
1.0.8
  -|Extension 
  -|Add Adv. Preset Custom Stepper Motor Drivers
  -|Add Status Logo FLSun
  -|Add TMC mode
1.0.5 
  -|Major Update & Reworking
  -| Add Adv. Preset Linear Pressure Control
  -| Add Adv. Preset Motion Modes
  -| Minor modes TFT Fixes
1.0.4
  -|Active differents Marlin Menus(Delta,PID,EndStop,UBL)
  -|Color Marlin TFT scale up
  -|improve buffer (128)
  -|STM32F1: Fix SDIO read errors (SDCard/TFT)
  -|Improve SD Card Read Speed
1.0.3
  -|Create new definition Pin for HISPEED Board
  -|Message connection Wifi on screen 
  -|Error messages separated
  -|Add Park
1.0.2
  -|Add firmware ESP3D (MKS Wifi)
  -|Add Pin sensor filament
  -|Add TFT Color 320x240
  -|Fix SD Read Errors
1.0.1|Initial commit
  

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
