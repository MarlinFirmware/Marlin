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

#define BOARD_INFO_NAME "BIGTREE SKR Mini E3 V1.2"

// Release PB3/PB4 (E0 STP/DIR) from JTAG pins
#define DISABLE_JTAG

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

#define FLASH_EEPROM_EMULATION
#define EEPROM_PAGE_SIZE     uint16(0x800) // 2KB
#define EEPROM_START_ADDRESS uint32(0x8000000 + 256 * 1024 - 2 * EEPROM_PAGE_SIZE)
#undef E2END
#define E2END                (EEPROM_PAGE_SIZE - 1) // 2KB

//
// Servos
//
#define SERVO0_PIN         PA1

//
// Limit Switches
//
#define X_STOP_PIN         PC0
#define Y_STOP_PIN         PC1
#define Z_STOP_PIN         PC2

//
// Z Probe must be this pins
//
#define Z_MIN_PROBE_PIN    PC14

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PC12
#endif

//
// Steppers
//
#define X_ENABLE_PIN       PB14
#define X_STEP_PIN         PB13
#define X_DIR_PIN          PB12

#define Y_ENABLE_PIN       PB11
#define Y_STEP_PIN         PB10
#define Y_DIR_PIN          PB2

#define Z_ENABLE_PIN       PB1
#define Z_STEP_PIN         PB0
#define Z_DIR_PIN          PC5

#define E0_ENABLE_PIN      PD2
#define E0_STEP_PIN        PB3
#define E0_DIR_PIN         PB4

#if HAS_TMC220x
  /**
   * TMC2208/TMC2209 stepper drivers
   */
  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN  PB15
  #define X_SERIAL_RX_PIN  PB15

  #define Y_SERIAL_TX_PIN  PC6
  #define Y_SERIAL_RX_PIN  PC6

  #define Z_SERIAL_TX_PIN  PC10
  #define Z_SERIAL_RX_PIN  PC10

  #define E0_SERIAL_TX_PIN PC11
  #define E0_SERIAL_RX_PIN PC11
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

#define NEOPIXEL_PIN       PC7  // LED driving pin

#define SD_DETECT_PIN      PC4

/**
 *                 _____
 *             5V | · · | GND
 *  (LCD_EN) PB7  | · · | PB8  (LCD_RS)
 *  (LCD_D4) PB9  | · · | PA10 (BTN_EN2)
 *          RESET | · · | PA9  (BTN_EN1)
 * (BTN_ENC) PB6  | · · | PB5  (BEEPER)
 *                 -----
 *                 EXP1
 */
#if HAS_SPI_LCD
  #define BEEPER_PIN       PB5
  #define BTN_ENC          PB6

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    PB8

    #define BTN_EN1        PA9
    #define BTN_EN2        PA10

    #define LCD_PINS_ENABLE PB7
    #define LCD_PINS_D4    PB9

  #else
    #error "Only CR10_STOCKDISPLAY is currently supported on the BIGTREE_SKR_MINI_E3 V1.1."
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
