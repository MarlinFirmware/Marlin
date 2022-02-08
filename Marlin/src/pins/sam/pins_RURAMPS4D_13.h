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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Ported sys0724 & Vynt
 */
#pragma once

/**
 * Arduino Mega? or Due with RuRAMPS4DUE pin assignments
 *
 * Applies to the following boards:
 *  RURAMPS4DUE      (Hotend0, Hotend1, Hotend2, Fan0, Fan1, Bed)
 *
 *  Differences between
 *     RADDS | RuRAMPS4DUE
 *           |
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "RuRAMPS4Due v1.3"

//
// Servos
//
#define SERVO0_PIN                             5
#define SERVO1_PIN                             3

//
// Limit Switches
//
#define X_MIN_PIN                             45
#define X_MAX_PIN                             39
#define Y_MIN_PIN                             46
#define Y_MAX_PIN                             41
#define Z_MIN_PIN                             47
#define Z_MAX_PIN                             43

//
// Steppers
//
#define X_STEP_PIN                            37  // Support Extension Board
#define X_DIR_PIN                             36
#define X_ENABLE_PIN                          31
#ifndef X_CS_PIN
  #define X_CS_PIN                            38
#endif

#define Y_STEP_PIN                            32  // Support Extension Board
#define Y_DIR_PIN                             35
#define Y_ENABLE_PIN                          31
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            34
#endif

#define Z_STEP_PIN                            30  // Support Extension Board
#define Z_DIR_PIN                              2
#define Z_ENABLE_PIN                          31
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            10
#endif

#define E0_STEP_PIN                           29
#define E0_DIR_PIN                            28
#define E0_ENABLE_PIN                         33
#ifndef E0_CS_PIN
  #define E0_CS_PIN                           14
#endif

#define E1_STEP_PIN                           22
#define E1_DIR_PIN                            24
#define E1_ENABLE_PIN                         26
#ifndef E1_CS_PIN
  #define E1_CS_PIN                           15
#endif

#define E2_STEP_PIN                           25
#define E2_DIR_PIN                            23
#define E2_ENABLE_PIN                         27
#ifndef E2_CS_PIN
  #define E2_CS_PIN                           61
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     49
#endif

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN               Y_MIN_PIN
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                          13
#define HEATER_1_PIN                          12
#define HEATER_2_PIN                          11
#define HEATER_BED_PIN                         7  // BED H1

#define FAN_PIN                                9
#define FAN1_PIN                               8
#define CONTROLLER_FAN_PIN                    -1

//
// Temperature Sensors
//
#define TEMP_0_PIN                             0  // ANALOG A0
#define TEMP_1_PIN                             1  // ANALOG A1
#define TEMP_2_PIN                             2  // ANALOG A2
#define TEMP_3_PIN                             3  // ANALOG A3
#define TEMP_BED_PIN                           4  // ANALOG A4

// The thermocouple uses Analog pins
#if ENABLED(VER_WITH_THERMOCOUPLE)                // Defined in Configuration.h
  #define TEMP_4_PIN                           5  // A5
  #define TEMP_5_PIN                           6  // A6 (Marlin 2.0 not support)
#endif

// SPI for MAX Thermocouple
/*
#if DISABLED(SDSUPPORT)
  #define TEMP_0_CS_PIN                       53
#else
  #define TEMP_0_CS_PIN                       49
#endif
*/

//
// Misc. Functions
//
#define SDSS                                   4  // 4,10,52 if using HW SPI.
#define LED_PIN                               -1  // 13 - HEATER_0_PIN
#define PS_ON_PIN                             -1  // 65

// MKS TFT / Nextion Use internal USART-1
#define TFT_LCD_MODULE_COM                     1
#define TFT_LCD_MODULE_BAUDRATE              115200

// ESP WiFi Use internal USART-2
#define ESP_WIFI_MODULE_COM                    2
#define ESP_WIFI_MODULE_BAUDRATE             115200
#define ESP_WIFI_MODULE_RESET_PIN             -1
#define PIGGY_GPIO_PIN                        -1

//
// EEPROM
//
#define MARLIN_EEPROM_SIZE                0x8000  // 32Kb (24lc256)
#define I2C_EEPROM                                // EEPROM on I2C-0
//#define EEPROM_SD                               // EEPROM on SDCARD
//#define SPI_EEPROM                              // EEPROM on SPI-0
//#define SPI_CHAN_EEPROM1        ?
//#define SPI_EEPROM1_CS_PIN      ?
// 2K EEPROM
//#define SPI_EEPROM2_CS_PIN      ?
// 32Mb FLASH
//#define SPI_FLASH_CS_PIN        ?

//
// LCD / Controller
//
#if HAS_WIRED_LCD

  #if ANY(RADDS_DISPLAY, IS_RRD_SC, IS_RRD_FG_SC)
    #define BEEPER_PIN                        62
    #define LCD_PINS_D4                       48
    #define LCD_PINS_D5                       50
    #define LCD_PINS_D6                       52
    #define LCD_PINS_D7                       53
    #define SD_DETECT_PIN                     51
  #endif

  #if EITHER(RADDS_DISPLAY, IS_RRD_SC)

    #define LCD_PINS_RS                       63
    #define LCD_PINS_ENABLE                   64

  #elif IS_RRD_FG_SC

    #define LCD_PINS_RS                       52
    #define LCD_PINS_ENABLE                   53

  #elif HAS_U8GLIB_I2C_OLED

    #define BEEPER_PIN                        62
    #define LCD_SDSS                          10
    #define SD_DETECT_PIN                     51

  #elif ENABLED(FYSETC_MINI_12864)

    #define BEEPER_PIN                        62
    #define DOGLCD_CS                         64
    #define DOGLCD_A0                         63

    //#define FORCE_SOFT_SPI                      // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

    #define LCD_RESET_PIN                     48  // Must be high or open for LCD to operate normally.

    #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
      #ifndef RGB_LED_R_PIN
        #define RGB_LED_R_PIN                 50  // D5
      #endif
      #ifndef RGB_LED_G_PIN
        #define RGB_LED_G_PIN                 52  // D6
      #endif
      #ifndef RGB_LED_B_PIN
        #define RGB_LED_B_PIN                 53  // D7
      #endif
    #elif ENABLED(FYSETC_MINI_12864_2_1)
      #define NEOPIXEL_PIN                    50  // D5
    #endif

  #elif ENABLED(MKS_MINI_12864)

    #define DOGLCD_A0                         52
    #define DOGLCD_CS                         50

    #define SD_DETECT_PIN                     51

  #endif

  #if IS_NEWPANEL
    #define BTN_EN1                           44
    #define BTN_EN2                           42
    #define BTN_ENC                           40
  #endif

  #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define BTN_ENC_EN               LCD_PINS_D7  // Detect the presence of the encoder
  #endif

#endif // HAS_WIRED_LCD
