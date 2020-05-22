/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME   "FYSETC AIO II"
#define BOARD_WEBSITE_URL "fysetc.com"

#define DISABLE_JTAG

#define pins_v2_20190128   // geo-f:add for new pins define

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Flash EEPROM Emulation
//
#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U) // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2KB
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PA1
#define Y_STOP_PIN                          PA0
#define Z_STOP_PIN                          PB14

//
// Filament runout
//
#ifdef pins_v2_20190128
  #define FIL_RUNOUT_PIN                    PB15
#else
  #define FIL_RUNOUT_PIN                    PB5
#endif

//
// Steppers
//
#define X_STEP_PIN                          PB8
#define X_DIR_PIN                           PB9
#define X_ENABLE_PIN                        PA8

#define Y_STEP_PIN                          PB2
#ifdef pins_v2_20190128
  #define Y_DIR_PIN                         PB3
#else
  #define Y_DIR_PIN                         PB0
#endif
#define Y_ENABLE_PIN                        PB1

#define Z_STEP_PIN                          PC0
#define Z_DIR_PIN                           PC1
#define Z_ENABLE_PIN                        PC2

#define E0_STEP_PIN                         PC15
#define E0_DIR_PIN                          PC14
#define E0_ENABLE_PIN                       PC13

//
// Stepper current PWM
//

// X:PA2 Y:PA3 Z:PB12 E:PB13 // changed for test
//#define MOTOR_CURRENT_PWM_XY_PIN          PA3
//#define MOTOR_CURRENT_PWM_Z_PIN           PA2   // PB12
//#define MOTOR_CURRENT_PWM_XY_PIN          PB6
//#define MOTOR_CURRENT_PWM_Z_PIN           PB7   // PB12
//#define MOTOR_CURRENT_PWM_E_PIN           -1    // PB13
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE 1500            // geo-f:old 2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {500, 500, 400} // geo-f:old 1300 1300 1250

// 采用 SDIO PCB从左到右数
// 1:PC10 - SDIO_D2
// 2:PC11 - SDIO_D3
// 3:PD2 - SDIO_CMD
// 4:VCC
// 5:PC12 - SDIO_CK
// 6:VDD
// 7:PC8 - SDIO_D0
// 8:PC9 - SDIO_D1
// 9:PA15 - SD_DETECT_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC7
#define HEATER_BED_PIN                      PC6
#ifndef FAN_PIN
  #define FAN_PIN                           PC8
#endif

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PC5   // Analog Input
#define TEMP_0_PIN                          PC4   // Analog Input

//
// Misc. Functions
//
#define SDSS                                PA4

//
// LCD Pins
//
#if HAS_SPI_LCD

  #define BEEPER_PIN                        PC9

  #if HAS_GRAPHICAL_LCD

    #define DOGLCD_A0                       PA15
    #ifdef pins_v2_20190128
      #define DOGLCD_CS                     PB5
    #else
      #define DOGLCD_CS                     PB7
    #endif

    //#define LCD_CONTRAST_INIT 190
    //#define LCD_SCREEN_ROT_90
    //#define LCD_SCREEN_ROT_180
    //#define LCD_SCREEN_ROT_270

  #endif

  // not connected to a pin
  #define SD_DETECT_PIN                     PC3

  #if ENABLED(NEWPANEL)
    // The encoder and click button
    #define BTN_EN1                         PC10
    #define BTN_EN2                         PC11
    #define BTN_ENC                         PC12
  #endif

  #ifdef pins_v2_20190128
    #define LCD_RESET_PIN                   PB4
    #ifndef RGB_LED_R_PIN
      #define RGB_LED_R_PIN                 PB0
    #endif
    #ifndef RGB_LED_G_PIN
      #define RGB_LED_G_PIN                 PB6
    #endif
    #ifndef RGB_LED_B_PIN
      #define RGB_LED_B_PIN                 PB7
    #endif
  #else
    #define LCD_RESET_PIN                   PB6
    #ifndef RGB_LED_R_PIN
      #define RGB_LED_R_PIN                 PB3
    #endif
    #ifndef RGB_LED_G_PIN
      #define RGB_LED_G_PIN                 PB4
    #endif
    #ifndef RGB_LED_B_PIN
      #define RGB_LED_B_PIN                 PB5
    #endif
  #endif

#endif
