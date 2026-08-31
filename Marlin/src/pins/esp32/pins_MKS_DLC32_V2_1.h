/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * MKS DLC32 V2.1 pin assignments
 * https://github.com/makerbase-mks/MKS-DLC32
 *
 * A 3-axis CNC / laser controller. No extruders, hotends, bed, or fans.
 * X, Y, Z step/dir and the shared driver-enable are driven through a 74HC595
 * fed by the I2S stepper stream, so those are expander pins (128 + bit).
 */

#include "env_validate.h"

#if EXTRUDERS || E_STEPPERS
  #error "MKS DLC32 has no E stepper driver. Set EXTRUDERS to 0."
#elif HAS_HOTEND || TEMP_SENSOR_BED
  #error "MKS DLC32 has no heater outputs. Set TEMP_SENSOR_BED to 0."
#endif

#define BOARD_INFO_NAME      "MKS DLC32 V2.1"
#define BOARD_WEBSITE_URL    "github.com/makerbase-mks/MKS-DLC32"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

// MAX_EXPANDER_BITS is set for this board in HAL/ESP32/inc/Conditionals_adv.h

//
// Limit Switches
//
#define X_STOP_PIN                            36  // Input only
#define Y_STOP_PIN                            35  // Input only
#define Z_STOP_PIN                            34  // Input only

//
// Probe
//
#define Z_MIN_PROBE_PIN                       22

//
// I2S stepper stream
//
#ifndef I2S_STEPPER_STREAM
  #define I2S_STEPPER_STREAM
#endif
#if ENABLED(I2S_STEPPER_STREAM)
  #define I2S_WS                              17
  #define I2S_BCK                             16
  #define I2S_DATA                            21
#endif

//
// Steppers - all on the I2S expander (74HC595)
//
#define X_STEP_PIN                           129  // i2so.1
#define X_DIR_PIN                            130  // i2so.2
#define X_ENABLE_PIN                         128  // i2so.0 - shared by all drivers

#define Y_STEP_PIN                           133  // i2so.5
#define Y_DIR_PIN                            134  // i2so.6
#define Y_ENABLE_PIN                X_ENABLE_PIN

#define Z_STEP_PIN                           131  // i2so.3
#define Z_DIR_PIN                            132  // i2so.4
#define Z_ENABLE_PIN                X_ENABLE_PIN

//
// I2C
//
#define I2C_SDA_PIN                            0
#define I2C_SCL_PIN                            4

//
// LED
//
#define LED_PIN                                2  // Wi-Fi debug LED

//
// M3/M4/M5 - Spindle/Laser Control
//
// The TTL header drives 0-5V for a laser. The spindle header switches ground.
//
#define SPINDLE_LASER_PWM_PIN                 32

/**
 * The ESP32 has two usable SPI buses:
 *   HSPI - GPIO 14 (SCK), 13 (MOSI), 12 (MISO), 15 (CS) - MicroSD card
 *   VSPI - GPIO 18 (SCK), 23 (MOSI), 19 (MISO),  5 (CS) - TFT display
 */

//
// MicroSD card (HSPI)
//
#define SD_SCK_PIN                            14
#define SD_MOSI_PIN                           13
#define SD_MISO_PIN                           12
#define SD_SS_PIN                             15
#define SD_DETECT_PIN                         39  // Input only

/**
 *                  ------                                 ------
 *  (BEEPER)   135 | 1  2 | NC              (LCD_MISO) 19 | 1  2 | 18 (LCD_SCK)
 *  (LCD_EN)    5* | 3  4 | 27* (LCD_RST)              NC | 3  4 | NC
 *  (TOUCH_CS) 26* | 5  6   NC                         NC | 5  6   23 (LCD_MOSI)
 *  (LCD_CS)   25* | 7  8 | 33  (LCD_RS)               NC | 7  8 | RESET
 *             GND | 9 10 | 5V                        GND | 9 10 | 3.3V
 *                  ------                                 ------
 *                   EXP1                                   EXP2
 *
 * * Buffered through an AHCT125. Output only - cannot be used as an input.
 */
#define EXP1_01_PIN                          135  // i2so.7 - shared with the BEEPER header
#define EXP1_02_PIN                            -1
#define EXP1_03_PIN                            5
#define EXP1_04_PIN                           27
#define EXP1_05_PIN                           26
#define EXP1_06_PIN                            -1
#define EXP1_07_PIN                           25
#define EXP1_08_PIN                           33

#define EXP2_01_PIN                           19
#define EXP2_02_PIN                           18
#define EXP2_03_PIN                            -1
#define EXP2_04_PIN                            -1
#define EXP2_05_PIN                            -1
#define EXP2_06_PIN                           23
#define EXP2_07_PIN                            -1
#define EXP2_08_PIN                            -1  // RESET

//
// Beeper
//
#define BEEPER_PIN                   EXP1_01_PIN

/**
 * TFT display on EXP1/EXP2 over VSPI
 *
 * The board ships with an MKS TS24-R V2.1 (320x240). An MKS TS35-R V2.0
 * (480x320) plugs into the same EXP1/EXP2 headers with identical wiring, so
 * the pins below serve both. Only MKS_TS35_V2_0 exists in Marlin today - the
 * TS24-R option arrives with the display support that follows this commit.
 */
#if ENABLED(MKS_TS35_V2_0)

  /**                      ------                                   ------
   *               BEEPER | 1  2 | BTN_ENC               SPI1_MISO | 1  2 | SPI1_SCK
   *     TFT_BKL / LCD_EN | 3  4 | TFT_RESET / LCD_RS      BTN_EN1 | 3  4 | SPI1_CS
   *    TOUCH_CS / LCD_D4 | 5  6   TOUCH_INT / LCD_D5      BTN_EN2 | 5  6   SPI1_MOSI
   *     SPI1_CS / LCD_D6 | 7  8 | SPI1_RS / LCD_D7       SPI1_RS  | 7  8 | RESET
   *                  GND | 9 10 | VCC                         GND | 9 10 | VCC
   *                       ------                                   ------
   *                        EXP1                                     EXP2
   */

  #define TFT_CS_PIN                 EXP1_07_PIN
  #define TFT_A0_PIN                 EXP1_08_PIN
  #define TFT_DC_PIN                  TFT_A0_PIN
  #define TFT_RESET_PIN              EXP1_04_PIN

  #define LCD_BACKLIGHT_PIN          EXP1_03_PIN
  #define TFT_BACKLIGHT_PIN    LCD_BACKLIGHT_PIN

  #define TFT_SCK_PIN                EXP2_02_PIN
  #define TFT_MISO_PIN               EXP2_01_PIN
  #define TFT_MOSI_PIN               EXP2_06_PIN

  #define LCD_USE_DMA_SPI
  #define TFT_BUFFER_WORDS                  2400

  #define TOUCH_CS_PIN               EXP1_05_PIN
  #define TOUCH_SCK_PIN               TFT_SCK_PIN
  #define TOUCH_MISO_PIN             TFT_MISO_PIN
  #define TOUCH_MOSI_PIN             TFT_MOSI_PIN
  // TOUCH_INT is not brought out on EXP1 (pin 6 is NC)

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE          1

#endif // MKS_TS35_V2_0
