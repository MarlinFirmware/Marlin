# Marlin for ET4

The Marlin for ET4 project is an effort to try to adapt the Anet ET4 motherboard and display for use with Marlin.
Anyone can contribute to completing this project. Feedback is also welcome.

## Build / Installation

DISCLAIMER: Not for production use. This is an unfinished project under development. I (@davidtgbe) am not responsible for what may happen to the motherboard or printer. Use at your own risk.

A flasher (stlink, jlink, bmp etc) is required to flash Marlin onto the board because it has a closed custom bootloader.

### Before flashing (recommended):

Make a backup of the firmware on the board first. At least back up the bootloader (0x08000000 - 0x08010000). That will allow you to recover the full stock firmware by:
  1. flashing the backed-up bootloader to the same address range (0x08000000 - 0x8010000), and
  2. flashing any of the available Anet binary firmwares (_i.e._, `et4.bin`) at address 0x08010000.

If you don't perform this step and end up "bricking" your board, you can get the stock ET4 firmware and bootloader linked in the Resources section below.

### Flashing the firmware:

There are several tutorials available for [stlink](https://www.cnx-software.com/2020/02/04/how-to-recover-from-a-bad-firmware-upgrade-on-anet-et4-3d-printer/)/[j-link](https://danielabalo.wordpress.com/flasear-anet-et4/) flashers.

There's no need to keep the stock bootloader on the board, so you can flash Marlin starting at address 0x8000000.
  1. Download the latest Marlin firmware. (Version 2.0.6.2 or newer.)
  2. Use the appropriate configuration files for your ET4 model (ET4, ET4 Pro, ET4+, ...). The affected settings include:
     - Driver model (A4988 vs. TMC2208)
     - Z endstop position (MIN vs. MAX)
     - Bed size
     - Bed leveling sensor
  3. For best results download the free "_Visual Studio Code_" and use the "_Auto Build Marlin_" extension with "_PlatformIO IDE_".
  4. Use your flasher to burn the firmware (`.elf` or `.bin` file) starting from address 0x08000000.

Connect your favorite serial host to the printer's serial port at 115200 baud to test it out!

## Resources

- Video: [Flash Marlin on ET4x with ST-Link](https://youtu.be/LUQe3hQBz2Q)
- Video: [Marlin on ET4 Demo](https://youtu.be/9ZN-0AMJHk0)
- [ET4 Telegram Spanish Group Resources](https://drive.google.com/drive/folders/1bVusF9dMh1H7c2JM5ZWlbn2tWRGKsHre)
- [ET4 Board and specs](https://es.aliexpress.com/item/4000571722465.html?spm=a2g0o.productlist.0.0.5c647634dDFWSV&algo_pvid=9a06cdcd-c1f2-45a0-adcf-36da50fefff7&algo_expid=9a06cdcd-c1f2-45a0-adcf-36da50fefff7-2&btsid=0ab6f83115911132482433653e39a1&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)

## Marlin for Anet ET Project status

### Working:
- XYZ Steppers
- Extruder
- SD Card
- USB comunication / pronterface
- TFT
- Filament runout detector.
- EEPROM
- Powerloss. See issues tab.

### In-progress
- PC/SD firmware load/update: There is no bootloader currently. Best option would be use stock ET4 bootloader.

### Needs a look
- All files at "Marlin/buildroot/share/PlatformIO/variants/ET4" should be adapted to ET4 Board. Especially `peripheralPins.c`, `variant.cpp`, `variant.h`, etc.

### Reporting bugs
Report bugs to the [Issues section of my Marlin fork](https://github.com/davidtgbe/Marlin/issues). Please be detailed regarding use-cases and other useful information like hardware and software context. For help with creating issues see [this](https://docs.github.com/en/enterprise/2.15/user/articles/creating-an-issue).

## Acknowledgements
- To the mates of the [Telegram Anet ET4 spanish group](https://t.me/anetet4esp), specially to @Solidnon, who lent his board for testing when the project was not even started.
- To @uwe and @mubes from Black Magic Probe team, and to Ebiroll (BMP/ESP32).
- To all contributors and testers of this branch and, specially, of Marlin master branch.

## Hardware
- [MCU: STM32F407VGT6 ARM CORTEX M4](https://www.st.com/resource/en/datasheet/dm00037051.pdf)
- DRIVERS: TMC2208 (silent) / A4988 (noisy)
- USB TO SERIAL CONVERTER: [CH340G](https://www.mpja.com/download/35227cpdata.pdf)
- FLASH: [WINBOND W25Q128JVSQ](https://www.winbond.com/resource-files/w25q128jv%20revf%2003272018%20plus.pdf) (128M-bit) Serial Flash memory
- EEPROM: [AT24C04C](https://datasheet.lcsc.com/szlcsc/1809192313_Microchip-Tech-AT24C04C-SSHM-T_C6205.pdf) (ATMLH744 04CM) 4 Kb
- LCD: [ST7789V](https://www.crystalfontz.com/controllers/- Sitronix/ST7789V/470/) | [STP320240_0280E2T](https://a.aliexpress.com/_dV4Bghv) (40P/1,5): ST7789 (YT280S008)
- TOUCH: [XPT2046](https://ldm-systems.ru/f/doc/catalog/HY-TFT-2,8/XPT2046.pdf)
- MOSFETS (BED/HOTEND): G90N04
- CLK: JF8.000 (8MHZ MCU EXT CLK)
- CLK: JF12.000 (12 MHZ USB-UART CLK)
- SS56: SCHOTTKY DIODE
- AMS1117 3.3 DN811: REGULATOR
- 030N06: MOSFETs
- A19T: TRANSISTOR
- XL2596S -5.0E1 83296: STEP DOWN DC CONVERTER 3A/150KHZ
- 293 [STG9834 / LM293DT](https://www.st.com/resource/en/datasheet/lm193.pdf): 2x Voltage comparators

## Pin Mapping
```
PB9  : E-STEP
PB8  : E-DIR
PE0  : E-ENABLE

PB6  : X-STEP
PB5  : X-DIR
PB7  : X-ENABLE

PB3  : Y-STEP
PD6  : Y-DIR
PB4  : Y-ENABLE

PA12 : Z-STEP
PA11 : Z-DIR
PA15 : Z-ENABLE

PE12 : Y-LIMIT
PC13 : X-LIMIT
PE11 : Z-LIMIT

PA4  : TEMP_BED
PA1  : TEMP_EXB1

PE1  : END_FAN
PE3  : LAY_FAN

PA0  : END_CONTROL
PE2  : BED_CONTROL

PC3  : LV_DET
PA2  : MAT_DET1
PA8 (provided by ANET) : POWER_LOSS_DET

PC10 : SDIO_D2
PC11 : SDIO_D3
PD2  : SDIO_CMD
PC12 : SDIO_CLK
PC8  : SDIO_D0
PC9  : SDIO_D1
PD3  : TF_DET

PA9  : USB_USART1_TX
PA10 : USB_USART1_RX

NRST (14) : RESET_BTN
PD12  : LED_D2

PB12  : WINBOND_CS
PB14  : WINBOND_DO
PB15  : WINBOND_DI
PB13  : WINBOND_CLK

GND   : EEPROM_A1
GND   : EEPROM_A2
PB11  : EEPROM_SDA
PB10  : EEPROM_SCL

PD7  : P1_1_LCD_9_CSX
PD5  : P1_2_LCD_11_WRX
PB2  : P1_3_TOUC_/CS
PE5  : P1_4_TOUCH_14_DIN
PE4  : P1_5_TOUCH_12_DOUT
PB0  : P1_6_TOUCH_16_DCLK
PB1  : P1_7_TOUC_/PENIRQ
PD4  : P1_8_LCD_12_RDX
GND  : P1_9
3.3V : P1_10

PE6  : P2_1_LCD_15_RESX
PD13 : P2_2_LCD_10_DCX
PD15 : P2_3_LCD_26_DB9
PD14 : P2_4_LCD_25_DB8
PD1  : P2_5_LCD_28_DB11
PD0  : P2_6_LCD_27_DB10
PE8  : P2_7_LCD_30_DB13
PE7  : P2_8_LCD_29_DB12
PE10 : P2_9_LCD_32_DB15
PE9  : P2_10_LCD_31_DB14
```
