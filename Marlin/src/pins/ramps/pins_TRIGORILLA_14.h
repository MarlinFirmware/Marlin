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

/**
 * Arduino Mega with RAMPS v1.4 for Anycubic
 * ATmega2560
 */

#define BOARD_INFO_NAME "Anycubic RAMPS 1.4"

//
// Servos
//
#if MB(TRIGORILLA_14_11)
  #define SERVO0_PIN                           5
  #define SERVO1_PIN                           4
  #define SERVO2_PIN                          11
  #define SERVO3_PIN                           6
#endif

//
// PWM FETS
//
#define MOSFET_B_PIN                          45  // HEATER1

//
// Heaters / Fans
//
#define FAN_PIN                                9  // FAN0
#define FAN1_PIN                               7  // FAN1
#define FAN2_PIN                              44  // FAN2
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN               FAN2_PIN
#endif

/**
 * Trigorilla Plugs (oriented with stepper plugs at the top)
 *
 *   SENSORS : GND GND GND GND
 *             A12 A15 A14 A13
 *            (D66 D69 D68 D67)
 *
 *       AUX : D42 GND 5V  (Chiron Y-STOP)
 *             D43 GND 5V  (Chiron Z-STOP)
 *
 *     UART3 : GND D15 D14 5V
 *                (RX3 TX3)
 *
 *       IIC : 12V GND D21 D20 GND 5V
 *                    (SCL SDA)
 *
 * END STOPS : D19 D18 D15 D14 D2  D3
 *             GND GND GND GND GND GND
 *             5V  5V  5V  5V  5V  5V
 */

/**               Expansion Headers
 *        ------                    ------
 *    37 | 1  2 | 35     (MISO) 50 | 1  2 | 52 (SCK)
 *    17 | 3  4 | 16            31 | 3  4 | 53
 *    23   5  6 | 25            33   5  6 | 51 (MOSI)
 *    27 | 7  8 | 29            49 | 7  8 | 41
 * (GND) | 9 10 | (5V)       (GND) | 9 10 | RESET
 *        ------                    ------
 *         EXP1                      EXP2
 */
#define EXP1_01_PIN                           37
#define EXP1_02_PIN                           35
#define EXP1_03_PIN                           17
#define EXP1_04_PIN                           16
#define EXP1_05_PIN                           23
#define EXP1_06_PIN                           25
#define EXP1_07_PIN                           27
#define EXP1_08_PIN                           29

#define EXP2_01_PIN                           50  // MISO
#define EXP2_02_PIN                           52  // SCK
#define EXP2_03_PIN                           31
#define EXP2_04_PIN                           53
#define EXP2_05_PIN                           33
#define EXP2_06_PIN                           51  // MOSI
#define EXP2_07_PIN                           49
#define EXP2_08_PIN                           41

//
// AnyCubic pin mappings
//
// Define the appropriate mapping option in Configuration.h:
// - TRIGORILLA_MAPPING_CHIRON
// - TRIGORILLA_MAPPING_I3MEGA
//

//#define ANYCUBIC_4_MAX_PRO_ENDSTOPS
#if ENABLED(ANYCUBIC_4_MAX_PRO_ENDSTOPS)
  #define X_MAX_PIN                           43  // AUX (2)
  #define Y_STOP_PIN                          19  // Z+
#elif EITHER(TRIGORILLA_MAPPING_CHIRON, TRIGORILLA_MAPPING_I3MEGA)
  // Chiron uses AUX header for Y and Z endstops
  #define Y_STOP_PIN                          42  // AUX (1)
  #define Z_STOP_PIN                          43  // AUX (2)
  #define Z2_MIN_PIN                          18  // Z-

  #ifndef Z_MIN_PROBE_PIN
    #define Z_MIN_PROBE_PIN                    2  // X+
  #endif

  #define CONTROLLER_FAN_PIN            FAN1_PIN

  #if ENABLED(POWER_LOSS_RECOVERY)
    #define OUTAGETEST_PIN                    79
    #define OUTAGECON_PIN                     58
  #endif

  #if ENABLED(TRIGORILLA_MAPPING_CHIRON)
    #if ENABLED(ANYCUBIC_LCD_CHIRON) && !defined(FIL_RUNOUT_PIN)
      #define FIL_RUNOUT_PIN         EXP2_05_PIN  // Chiron Standard Adapter
    #endif
    #define HEATER_BED_PIN          MOSFET_B_PIN  // HEATER1
  #endif

  #ifndef FIL_RUNOUT_PIN
    #define FIL_RUNOUT_PIN                    19  // Z+
  #endif

  #if EITHER(TRIGORILLA_MAPPING_CHIRON, SWAP_Z_MOTORS)
    // Chiron and some Anycubic i3 MEGAs swap Z steppers
    #define Z_STEP_PIN                        36
    #define Z_DIR_PIN                         34
    #define Z_ENABLE_PIN                      30
    #define Z_CS_PIN                          44

    #define Z2_STEP_PIN                       46
    #define Z2_DIR_PIN                        48
    #define Z2_ENABLE_PIN                     62
    #define Z2_CS_PIN                         40
  #endif
#endif

#if EITHER(ANYCUBIC_LCD_CHIRON, ANYCUBIC_LCD_I3MEGA)
  #ifndef BEEPER_PIN
    #define BEEPER_PIN               EXP2_03_PIN  // Chiron Standard Adapter
  #endif
  #define SD_DETECT_PIN              EXP2_07_PIN  // Chiron Standard Adapter
#endif

#if HAS_TMC_UART
  #ifndef X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN           SERVO1_PIN
  #endif
  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN           SERVO0_PIN
  #endif
  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN           SERVO3_PIN
  #endif
  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN          SERVO2_PIN
  #endif
#endif

#include "pins_RAMPS.h"
