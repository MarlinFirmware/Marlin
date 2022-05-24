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
 */
#pragma once

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "MKS Robin Lite only supports 1 hotend / E stepper."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "MKS Robin Lite"
#endif
#define BOARD_WEBSITE_URL "github.com/makerbase-mks"

#define BOARD_NO_NATIVE_USB

//#define DISABLE_DEBUG
#define DISABLE_JTAG

//
// Limit Switches
//
#define X_STOP_PIN                          PC13
#define Y_STOP_PIN                          PC0
#define Z_MIN_PIN                           PC12
#define Z_MAX_PIN                           PB9

//
// Steppers
//
#define X_STEP_PIN                          PC6
#define X_DIR_PIN                           PB12
#define X_ENABLE_PIN                        PB10

#define Y_STEP_PIN                          PB11
#define Y_DIR_PIN                           PB2
#define Y_ENABLE_PIN                        PB10

#define Z_STEP_PIN                          PB1
#define Z_DIR_PIN                           PC5
#define Z_ENABLE_PIN                        PB10

#define E0_STEP_PIN                         PC4
#define E0_DIR_PIN                          PA5
#define E0_ENABLE_PIN                       PA4

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC9
#define FAN_PIN                             PA8
#define HEATER_BED_PIN                      PC8

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PA1
#define TEMP_0_PIN                          PA0

#define FIL_RUNOUT_PIN                      PB8   // MT_DET

/**                ------
 *   (BEEPER) PD2 |10  9 | PB3  (BTN_ENC)
 *  (BTN_EN1) PB5 | 8  7 | PA11 (RESET?)
 *  (BTN_EN2) PB4   6  5 | PC1  (LCD_D4)
 *   (LCD_RS) PC3 | 4  3 | PC2  (LCD_EN)
 *            GND | 2  1 | 5V
 *                 ------
 *               "E3" EXP1
 */
#define E3_EXP1_01_PIN                      -1    // 5V
#define E3_EXP1_02_PIN                      -1    // GND
#define E3_EXP1_03_PIN                      PC2
#define E3_EXP1_04_PIN                      PC3
#define E3_EXP1_05_PIN                      PC1
#define E3_EXP1_06_PIN                      PB4
#define E3_EXP1_07_PIN                      PA11  // RESET?
#define E3_EXP1_08_PIN                      PB5
#define E3_EXP1_09_PIN                      PB3
#define E3_EXP1_10_PIN                      PD2

//
// LCD Pins
//
#if HAS_WIRED_LCD
  #define BEEPER_PIN              E3_EXP1_10_PIN
  #define BTN_ENC                 E3_EXP1_09_PIN
  #define LCD_PINS_RS             E3_EXP1_04_PIN

  #define BTN_EN1                 E3_EXP1_08_PIN
  #define BTN_EN2                 E3_EXP1_06_PIN

  #define LCD_PINS_ENABLE         E3_EXP1_03_PIN

  #if ENABLED(MKS_MINI_12864)

    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   -1
    #define DOGLCD_A0             E3_EXP1_05_PIN
    #define DOGLCD_CS             E3_EXP1_03_PIN
    #define DOGLCD_SCK                      PB13
    #define DOGLCD_MOSI                     PB15

  #else                                           // !MKS_MINI_12864

    #define LCD_PINS_D4           E3_EXP1_05_PIN
    #if IS_ULTIPANEL
      #define LCD_PINS_D5                   -1
      #define LCD_PINS_D6                   -1
      #define LCD_PINS_D7                   -1
    #endif

  #endif // !MKS_MINI_12864

  // Alter timing for graphical display
  #if IS_U8GLIB_ST7920
    #define BOARD_ST7920_DELAY_1             125
    #define BOARD_ST7920_DELAY_2             125
    #define BOARD_ST7920_DELAY_3             125
  #endif

#endif // HAS_WIRED_LCD

// Motor current PWM pins
#define MOTOR_CURRENT_PWM_XY_PIN            PB0
#define MOTOR_CURRENT_PWM_Z_PIN             PA7
#define MOTOR_CURRENT_PWM_E_PIN             PA6
#define MOTOR_CURRENT_PWM_RANGE (65535/10/3.3) // (255 * (1000mA / 65535)) * 257 = 1000 is equal 1.6v Vref in turn equal 1Amp
#define DEFAULT_PWM_MOTOR_CURRENT { 1000, 1000, 1000 } // 1.05Amp per driver, here is XY, Z and E. This values determined empirically.

//
// SD Card
//
#define SD_DETECT_PIN                       PC10

//
// SPI
//
#define SPI_DEVICE                             2
#define SD_SCK_PIN                          PB13
#define SD_MISO_PIN                         PB14
#define SD_MOSI_PIN                         PB15
#define SD_SS_PIN                           PA15

// EXP1 replace LCD with keys for EasyThreeD ET4000+ Mainboard
#if ENABLED(EASYTHREED_UI)
  #define BTN_HOME                E3_EXP1_04_PIN  // INPUT_PULLUP (unused)
  #define BTN_FEED                E3_EXP1_09_PIN  // Run E Forward
  #define BTN_RETRACT             E3_EXP1_08_PIN  // Run E Backward
  #define BTN_PRINT               E3_EXP1_07_PIN  // Start File Print
  #define BTN_HOME_GND            E3_EXP1_03_PIN  // OUTPUT (LOW)
  #define BTN_FEED_GND            E3_EXP1_06_PIN  // OUTPUT (LOW)
  #define BTN_RETRACT_GND         E3_EXP1_05_PIN  // OUTPUT (LOW)
  #define EASYTHREED_LED_PIN      E3_EXP1_10_PIN  // Indicator LED
#endif
