/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#ifndef TARGET_STM32F1
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME "BIGTREE SKR E3 DIP V1.0"

// Release PB3/PB4 (TMC_SW Pins) from JTAG pins
#define DISABLE_JTAG

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

#define FLASH_EEPROM_EMULATION
#define EEPROM_PAGE_SIZE     uint16(0x800) // 2KB
#define EEPROM_START_ADDRESS uint32(0x8000000 + (STM32_FLASH_SIZE) * 1024 - 2 * EEPROM_PAGE_SIZE)
#undef E2END
#define E2END                (EEPROM_PAGE_SIZE - 1) // 2KB

//
// Servos
//
#define SERVO0_PIN         PA1

//
// Limit Switches
//
#define X_STOP_PIN         PC1
#define Y_STOP_PIN         PC0
#define Z_STOP_PIN         PC15

//
// Z Probe must be this pin
//
#define Z_MIN_PROBE_PIN    PC14

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PC2
#endif

//
// Steppers
//
#define X_ENABLE_PIN       PC7
#define X_STEP_PIN         PC6
#define X_DIR_PIN          PB15
#ifndef X_CS_PIN
  #define X_CS_PIN         PC10
#endif

#define Y_ENABLE_PIN       PB14
#define Y_STEP_PIN         PB13
#define Y_DIR_PIN          PB12
#ifndef Y_CS_PIN
  #define Y_CS_PIN         PC11
#endif

#define Z_ENABLE_PIN       PB11
#define Z_STEP_PIN         PB10
#define Z_DIR_PIN          PB2
#ifndef Z_CS_PIN
  #define Z_CS_PIN         PC12
#endif

#define E0_ENABLE_PIN      PB1
#define E0_STEP_PIN        PB0
#define E0_DIR_PIN         PC5
#ifndef E0_CS_PIN
  #define E0_CS_PIN        PD2
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    PB5
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    PB4
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     PB3
  #endif
#endif

#if HAS_TMC220x
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define E0_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN  PC10
  #define X_SERIAL_RX_PIN  PC10

  #define Y_SERIAL_TX_PIN  PC11
  #define Y_SERIAL_RX_PIN  PC11

  #define Z_SERIAL_TX_PIN  PC12
  #define Z_SERIAL_RX_PIN  PC12

  #define E0_SERIAL_TX_PIN PD2
  #define E0_SERIAL_RX_PIN PD2

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA0   // Analog Input
#define TEMP_BED_PIN       PC3   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN       PC8   // EXTRUDER
#define HEATER_BED_PIN     PC9   // BED
#define FAN_PIN            PA8

//
// USB connect control
//
#define USB_CONNECT_PIN    PC13
#define USB_CONNECT_INVERTING false

#define SD_DETECT_PIN      PC4

/**
 *                 _____
 *             5V | · · | GND
 *  (LCD_EN) PB7  | · · | PB8  (LCD_RS)
 *  (LCD_D4) PB9  | · · | PA10 (BTN_EN2)
 *          RESET | · · | PA9  (BTN_EN1)
 * (BTN_ENC) PB6  | · · | PA15 (BEEPER)
 *                 -----
 *                 EXP1
 */
#if HAS_SPI_LCD
  #define BEEPER_PIN       PA15
  #define BTN_ENC          PB6

  #if ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS    PB8

    #define BTN_EN1        PA9
    #define BTN_EN2        PA10

    #define LCD_PINS_ENABLE PB7
    #define LCD_PINS_D4    PB9

  #elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

    /** Creality Ender-2 display pinout
     *                   _____
     *               5V | · · | GND
     *      (MOSI) PB7  | · · | PB8  (LCD_RS)
     *    (LCD_A0) PB9  | · · | PA10 (BTN_EN2)
     *            RESET | · · | PA9  (BTN_EN1)
     *   (BTN_ENC) PB6  | · · | PA15 (SCK)
     *                   -----
     *                    EXP1
     */

    #define BTN_EN1      PA9
    #define BTN_EN2      PA10
    #define DOGLCD_CS    PB8
    #define DOGLCD_A0    PB9
    #define DOGLCD_SCK   PA15
    #define DOGLCD_MOSI  PB7
    #define FORCE_SOFT_SPI
    #define LCD_BACKLIGHT_PIN -1

  #else
    #error "Only CR10_STOCKDISPLAY and MKS_MINI_12864 are currently supported on the BIGTREE_SKR_E3_DIP."
  #endif

#endif // HAS_SPI_LCD

//
// SD Support
//
#define HAS_ONBOARD_SD

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION ONBOARD
#endif

#define ON_BOARD_SPI_DEVICE 1    //SPI1
#define ONBOARD_SD_CS_PIN  PA4   // Chip select for "System" SD card
