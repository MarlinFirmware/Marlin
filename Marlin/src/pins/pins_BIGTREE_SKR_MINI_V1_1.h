/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifndef TARGET_STM32F1
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "BIGTREE SKR mini V1.1"
#endif

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Limit Switches
//
#define X_MIN_PIN          PC2
#define X_MAX_PIN          PA2
#define Y_MIN_PIN          PC1
#define Y_MAX_PIN          PA1
#define Z_MIN_PIN          PC0
#define Z_MAX_PIN          PC3

//
// Steppers
//

/**
 * TODO: Currently using same Enable pin for all steppers.
 */

#define X_STEP_PIN         PC6
#define X_DIR_PIN          PC7
#define X_ENABLE_PIN       PB15

#define Y_STEP_PIN         PB13
#define Y_DIR_PIN          PB14
#define Y_ENABLE_PIN       PB12

#define Z_STEP_PIN         PB10
#define Z_DIR_PIN          PB11
#define Z_ENABLE_PIN       PB2

#define E0_STEP_PIN        PC5
#define E0_DIR_PIN         PB0
#define E0_ENABLE_PIN      PC4

#if ENABLED(TMC_USE_SW_SPI)
  #define TMC_SW_SCK       PB3
  #define TMC_SW_MISO      PB4
  #define TMC_SW_MOSI      PB5
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA8
#define FAN_PIN            PC8
#define HEATER_BED_PIN     PC9

//
// Temperature Sensors
//
#define TEMP_BED_PIN         PB1   // Analog Input
#define TEMP_0_PIN           PA0   // Analog Input

//
// LCD Pins
//

/**
 *               _____                                             _____
 *           NC | · · | GND                                    5V | · · | GND
 *        RESET | · · | 1.31(SD_DETECT)             (LCD_D7) 1.23 | · · | 1.22 (LCD_D6)
 *   (MOSI)0.18 | · · | 3.25(BTN_EN2)               (LCD_D5) 1.21 | · · | 1.20 (LCD_D4)
 *  (SD_SS)0.16 | · · | 3.26(BTN_EN1)               (LCD_RS) 1.19 | · · | 1.18 (LCD_EN)
 *    (SCK)0.15 | · · | 0.17(MISO)                 (BTN_ENC) 0.28 | · · | 1.30 (BEEPER)
 *               ￣￣                                               ￣￣
 *               EXP2                                              EXP1
 */

#if ENABLED(ULTRA_LCD)

  #define BEEPER_PIN       PC10
  #define BTN_ENC          PC11
  #define LCD_PINS_RS      PC12

  #define BTN_EN1          PD2
  #define BTN_EN2          PB8

  #define LCD_PINS_ENABLE  PB6
  #define LCD_PINS_D4      PC13

  #if ENABLED(ULTIPANEL)
    #define LCD_PINS_D5    PB7
    #define LCD_PINS_D6    PC15
    #define LCD_PINS_D7    PC14
  #endif

#endif // ULTRA_LCD

//
// SD Card
//

// Marlin uses the SD drive attached to the LCD
//#define STM32_SD_LCD

#ifdef STM32_SD_LCD
  #define SD_DETECT_PIN    PB9
  #define ENABLE_SPI3
#else
  #define SD_DETECT_PIN    PA3
#endif
