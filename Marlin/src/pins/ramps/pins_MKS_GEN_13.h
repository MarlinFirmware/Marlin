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

/**
 * Arduino Mega with RAMPS v1.4 adjusted pin assignments
 *
 *  MKS GEN v1.3  (Extruder, Fan, Bed)
 *  MKS GEN v1.3  (Extruder, Extruder, Fan, Bed)
 *  MKS GEN v1.4  (Extruder, Fan, Bed)
 *  MKS GEN v1.4  (Extruder, Extruder, Fan, Bed)
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS GEN 1.3/1.4 supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "MKS GEN >= v1.3"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN                           7

//
// PSU / SERVO
//
// If PSU_CONTROL is specified, always hijack Servo 3
//
#if ENABLED(PSU_CONTROL)
  #define SERVO3_PIN                          -1
  #define PS_ON_PIN                            4
#endif

#include "pins_RAMPS.h"

//
// LCD / Controller
//
#if ANY(VIKI2, miniVIKI)
  /**
   * VIKI2 Has two groups of wires with...
   *
   * +Vin     + Input supply, requires 120ma for LCD and mSD card
   * GND      Ground Pin
   * MOSI     Data input for LCD and SD
   * MISO     Data output for SD
   * SCK      Clock for LCD and SD
   * AO       Reg. Sel for LCD
   * LCS      Chip Select for LCD
   * SDCS     Chip Select for SD
   * SDCD     Card Detect pin for SD
   * ENCA     Encoder output A
   * ENCB     Encoder output B
   * ENCBTN   Encoder button switch
   *
   * BTN      Panel mounted button switch
   * BUZZER   Piezo buzzer
   * BLUE-LED Blue LED ring pin (3 to 5v, mosfet buffered)
   * RED-LED  Red LED ring pin (3 to 5v, mosfet buffered)
   *
   * This configuration uses the following arrangement:
   *
   * EXP1 D37 = EN2   D35 = EN1     EXP2 D50 = MISO  D52 = SCK
   *      D17 = BLUE  D16 = RED          D31 = ENC   D53 = SDCS
   *      D23 = KILL  D25 = BUZZ         D33 = ---   D51 = MOSI
   *      D27 = A0    D29 = LCS          D49 = SDCD  RST = ---
   *      GND = GND   5V  = 5V           GND = ---   D41 = ---
   */

  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #undef DOGLCD_A0
  #undef DOGLCD_CS
  #undef SD_DETECT_PIN
  #undef BEEPER_PIN
  #undef KILL_PIN
  #undef STAT_LED_RED_PIN
  #undef STAT_LED_BLUE_PIN

  //
  // VIKI2 12-wire lead
  //

  // orange/white          SDCD
  #define SD_DETECT_PIN                       49

  // white                 ENCA
  #define BTN_EN1                             35

  // green                 ENCB
  #define BTN_EN2                             37

  // purple                ENCBTN
  #define BTN_ENC                             31

  // brown                 A0
  #define DOGLCD_A0                           27

  // green/white           LCS
  #define DOGLCD_CS                           29

                        // 50    gray   MISO
                        // 51    yellow MOSI
                        // 52    orange SCK

  // blue                  SDCS
  //#define SDSS                              53

  //
  // VIKI2 4-wire lead
  //

  // blue                  BTN
  #define KILL_PIN                            23

  // green                 BUZZER
  #define BEEPER_PIN                          25

  // yellow                RED-LED
  #define STAT_LED_RED_PIN                    16

  // white                 BLUE-LED
  #define STAT_LED_BLUE_PIN                   17

#endif
