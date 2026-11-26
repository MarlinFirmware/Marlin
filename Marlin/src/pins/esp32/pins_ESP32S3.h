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
 * Espressif ESP32-S3 (Tensilica Xtensa LX7) pin assignments
 * for an ESP32-S3-DevKitC-1 or similar dev board.
 *
 * Avoided: GPIO 0, 3, 45, 46 (strapping), 19/20 (native USB),
 * 26-32 (flash), 33-37 (octal PSRAM on N8R8/N16R8 modules),
 * 38/48 (RGB LED), 43/44 (UART0 to the USB-UART bridge).
 * Thermistors use ADC1 (GPIO 1-10), which keeps working with WiFi on.
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Espressif ESP32-S3"

//
// Limit Switches
//
#define X_MIN_PIN                             39
#define Y_MIN_PIN                             40
#define Z_MIN_PIN                             41

//
// Steppers
//
#if ENABLED(I2S_STEPPER_STREAM)

  // I2S stream to 74HC595 shift registers (expander pins 128+)
  #define I2S_WS                               4  // 74HC595 latch (STCP)
  #define I2S_BCK                              5  // 74HC595 clock (SHCP)
  #define I2S_DATA                             6  // 74HC595 data (DS)

  #define X_STEP_PIN                         128
  #define X_DIR_PIN                          129
  #define X_ENABLE_PIN                       130

  #define Y_STEP_PIN                         131
  #define Y_DIR_PIN                          132
  #define Y_ENABLE_PIN                       133

  #define Z_STEP_PIN                         134
  #define Z_DIR_PIN                          135
  #define Z_ENABLE_PIN                       136

  #define E0_STEP_PIN                        137
  #define E0_DIR_PIN                         138
  #define E0_ENABLE_PIN                      139

#else

  #define X_STEP_PIN                           4
  #define X_DIR_PIN                            5
  #define X_ENABLE_PIN                        21

  #define Y_STEP_PIN                           6
  #define Y_DIR_PIN                            7
  #define Y_ENABLE_PIN              X_ENABLE_PIN

  #define Z_STEP_PIN                          15
  #define Z_DIR_PIN                           16
  #define Z_ENABLE_PIN              X_ENABLE_PIN

  #define E0_STEP_PIN                         17
  #define E0_DIR_PIN                          18
  #define E0_ENABLE_PIN             X_ENABLE_PIN

#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                             1  // Analog Input (ADC1)
#define TEMP_BED_PIN                           2  // Analog Input (ADC1)

//
// Heaters / Fans
//
#define HEATER_0_PIN                          14
#define HEATER_BED_PIN                        47
#define FAN0_PIN                              42

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                           9
#endif

//
// MicroSD card (FSPI default pins)
//
#define SD_SS_PIN                             10
#define SD_MOSI_PIN                           11
#define SD_SCK_PIN                            12
#define SD_MISO_PIN                           13

//
// Onboard WS2812 RGB LED (GPIO 38 on some DevKitC-1 revisions)
//
#ifndef BOARD_NEOPIXEL_PIN
  #define BOARD_NEOPIXEL_PIN                  48
#endif
