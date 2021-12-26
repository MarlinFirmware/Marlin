/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * MRR ESPE pin assignments
 * MRR ESPE is a 3D printer control board based on the ESP32 microcontroller.
 * Supports 5 stepper drivers (using I2S stepper stream), heated bed,
 * single hotend, and LCD controller.
 */

#include "env_validate.h"

#if EXTRUDERS > 2 || E_STEPPERS > 2
  #error "MKS ESP Nano only supports two E Steppers. Comment out this line to continue."
#elif HOTENDS > 2
  #error "MKS ESP Nano only supports two hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME      "MKS TinyBee"
#define BOARD_WEBSITE_URL    "https://github.com/makerbase-mks"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// Servos
//
#define SERVO0_PIN                             2

//
// Limit Switches
//
#define X_STOP_PIN                            33
#define Y_STOP_PIN                            32
#define Z_STOP_PIN                            22
//#define FIL_RUNOUT_PIN                      35

//
// Enable I2S stepper stream
//
#define I2S_STEPPER_STREAM
#if ENABLED(I2S_STEPPER_STREAM)
  #define I2S_WS                              26
  #define I2S_BCK                             25
  #define I2S_DATA                            27
  #if ENABLED(LIN_ADVANCE)
    #error "I2S stream is currently incompatible with LIN_ADVANCE."
  #endif
#endif

//
// Steppers
//
#define X_STEP_PIN                           129
#define X_DIR_PIN                            130
#define X_ENABLE_PIN                         128

#define Y_STEP_PIN                           132
#define Y_DIR_PIN                            133
#define Y_ENABLE_PIN                         131

#define Z_STEP_PIN                           135
#define Z_DIR_PIN                            136
#define Z_ENABLE_PIN                         134

#define E0_STEP_PIN                          138
#define E0_DIR_PIN                           139
#define E0_ENABLE_PIN                        137

#define E1_STEP_PIN                          141
#define E1_DIR_PIN                           142
#define E1_ENABLE_PIN                        140

#define Z2_STEP_PIN                          141
#define Z2_DIR_PIN                           142
#define Z2_ENABLE_PIN                        140

//
// Temperature Sensors
//
#define TEMP_0_PIN                            36  // Analog Input
#define TEMP_1_PIN                            34  // Analog Input, you need set R6=0Ω and R7=NC
#define TEMP_BED_PIN                          39  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                         145
#define HEATER_1_PIN                         146
#define FAN_PIN                              147
#define FAN1_PIN                             148
#define HEATER_BED_PIN                       144

//#define CONTROLLER_FAN_PIN                 148
//#define E0_AUTO_FAN_PIN                    148  // need to update Configuration_adv.h @section extruder
//#define E1_AUTO_FAN_PIN                    149  // need to update Configuration_adv.h @section extruder

//
// MicroSD card
//
#define SD_MOSI_PIN                           23
#define SD_MISO_PIN                           19
#define SD_SCK_PIN                            18
#define SDSS                                   5
#define SD_DETECT_PIN                         34  // IO34 default is SD_DET signal(Jump to SDDET)
#define USES_SHARED_SPI                           // SPI is shared by SD card with TMC SPI drivers

/**
 *                ------                                 ------
 *  (BEEPER) 149 |10  9 | 13 (BTN_ENC)    (SPI MISO) 19 |10  9 | 18 (SPI SCK)
 *  (LCD_EN)  21 | 8  7 |  4 (LCD_RS)      (BTN_EN1) 14 | 8  7 |  5 (SPI CS)
 *  (LCD_D4)   0 | 6  5   16 (LCD_D5)      (BTN_EN2) 12 | 6  5   23 (SPI MOSI)
 *  (LCD_D6)  15 | 4  3 | 17 (LCD_D7)      (SPI_DET) 34 | 4  3 | RESET
 *           GND | 2  1 | 5V                        GND | 2  1 | 3.3V
 *                ------                                 ------
 *                 EXP1                                   EXP2
 */

#define EXP1_03_PIN                           17
#define EXP1_04_PIN                           15
#define EXP1_05_PIN                           16
#define EXP1_06_PIN                            0
#define EXP1_07_PIN                            4
#define EXP1_08_PIN                           21
#define EXP1_09_PIN                           13
#define EXP1_10_PIN                          149

#define EXP2_03_PIN                           -1  // RESET
#define EXP2_04_PIN                           34
#define EXP2_05_PIN                           23
#define EXP2_06_PIN                           12
#define EXP2_07_PIN                            5
#define EXP2_08_PIN                           14
#define EXP2_09_PIN                           18
#define EXP2_10_PIN                           19

#if HAS_WIRED_LCD

  #define BEEPER_PIN                         149
  #define BTN_ENC                             13
  #define LCD_PINS_ENABLE                     21
  #define LCD_PINS_RS                          4
  #define BTN_EN1                             14
  #define BTN_EN2                             12
  #define LCD_BACKLIGHT_PIN                   -1

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)

    //#define LCD_BACKLIGHT_PIN               -1
    //#define LCD_RESET_PIN                   -1
    #define DOGLCD_A0                         15
    #define DOGLCD_CS                         16
    //#define DOGLCD_SCK                      19
    //#define DOGLCD_MOSI                     23

    // Required for MKS_MINI_12864 with this board
    //#define MKS_LCD12864B

  #elif ENABLED(MKS_MINI_12864_V3)

    #define DOGLCD_CS                EXP1_08_PIN
    #define DOGLCD_A0                EXP1_07_PIN
    #define LCD_PINS_DC                DOGLCD_A0
    #define LCD_BACKLIGHT_PIN                 -1
    #define LCD_RESET_PIN            EXP1_06_PIN
    #define NEOPIXEL_PIN             EXP1_05_PIN
    #define DOGLCD_MOSI              EXP2_05_PIN
    #define DOGLCD_SCK               EXP2_09_PIN
    #if SD_CONNECTION_IS(ONBOARD)
      #define FORCE_SOFT_SPI
    #endif

  #else // !MKS_MINI_12864

    #define LCD_PINS_D4                        0
    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
      #define LCD_PINS_D5                     16
      #define LCD_PINS_D6                     15
      #define LCD_PINS_D7                     17
    #endif

    #define BOARD_ST7920_DELAY_1              96
    #define BOARD_ST7920_DELAY_2              48
    #define BOARD_ST7920_DELAY_3             600

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD
