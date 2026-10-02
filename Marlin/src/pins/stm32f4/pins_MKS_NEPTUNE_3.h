/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// MKS Neptune 3
//

// Avoid conflict with TIMER_TONE
#define ALLOW_STM32F4
#include "env_validate.h"

#define BOARD_INFO_NAME "MKS Neptune 3"

//
// Release PB4 (Z_DIR_PIN) from JTAG NRST role
//
//#define DISABLE_DEBUG

//
// Servos
//
#define SERVO0_PIN                          PA8   // BLTOUCH

//
// Limit Switches
//
//#define ZNP_TEST
#ifdef ZNP_TEST
  #define X_DIAG_PIN                        PC14  // Z+
#else
  #define X_DIAG_PIN                        PA13  // X-
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PC14  // PB9
#endif

//
// Steppers
//
#define E1_ENABLE_PIN                       PC5
#define E1_STEP_PIN                         PC4
#define E1_DIR_PIN                          PA4

//
// Temperature Sensors
//
#define TEMP_1_PIN                          PC2   // TH2

//
// Fans
//
#define FAN0_PIN                            PB0   // FAN
//#define FAN1_PIN                          PA7   // FAN1

#if NEED_TOUCH_PINS
  #define TOUCH_CS_PIN                      PA7   // SPI2_NSS
  #define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
  #define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
  #define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI
#endif

#include "pins_MKS_NEPTUNE_3_common.h"
