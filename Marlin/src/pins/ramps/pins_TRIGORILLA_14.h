/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Arduino Mega with RAMPS v1.4 for Anycubic
 */

#define BOARD_INFO_NAME "Anycubic RAMPS 1.4"

//
// Servos
//
#if MB(TRIGORILLA_14_11)
  #define SERVO0_PIN        5
  #define SERVO1_PIN        4
  #define SERVO2_PIN       11
  #define SERVO3_PIN        6
#endif

//
// Custom Limit Switches
//
//#define ANYCUBIC_4_MAX_PRO_ENDSTOPS
#if ENABLED(ANYCUBIC_4_MAX_PRO_ENDSTOPS)
  #define X_MAX_PIN        43
  #define Y_MIN_PIN        19
#endif

// Labeled pins
#define TRIGORILLA_HEATER_BED_PIN  8
#define TRIGORILLA_HEATER_0_PIN   10
#define TRIGORILLA_HEATER_1_PIN   45   // Anycubic Kossel: Unused

#define TRIGORILLA_FAN0_PIN        9   // Anycubic Kossel: Usually the part cooling fan
#define TRIGORILLA_FAN1_PIN        7   // Anycubic Kossel: Unused
#define TRIGORILLA_FAN2_PIN       44   // Anycubic Kossel: Hotend fan

// Remap MOSFET pins to common usages:

#define RAMPS_D10_PIN    TRIGORILLA_HEATER_0_PIN // HEATER_0_PIN is always RAMPS_D10_PIN in pins_RAMPS.h

#if HOTENDS > 1   // EEF and EEB
  #define RAMPS_D9_PIN   TRIGORILLA_HEATER_1_PIN
  #if !TEMP_SENSOR_BED
    // EEF
    #define RAMPS_D8_PIN TRIGORILLA_FAN0_PIN
  #else
    // EEB
    #define RAMPS_D8_PIN TRIGORILLA_HEATER_BED_PIN
    #define FAN_PIN      TRIGORILLA_FAN0_PIN    // Override pin 4 in pins_RAMPS.h
  #endif
#elif TEMP_SENSOR_BED
  // EFB (Anycubic Kossel default)
  #define RAMPS_D9_PIN   TRIGORILLA_FAN0_PIN
  #define RAMPS_D8_PIN   TRIGORILLA_HEATER_BED_PIN
#else
  // EFF
  #define RAMPS_D9_PIN   TRIGORILLA_FAN1_PIN
  #define RAMPS_D8_PIN   TRIGORILLA_FAN0_PIN
#endif

#if HOTENDS > 1 || TEMP_SENSOR_BED      // EEF, EEB, EFB
  #define FAN1_PIN       TRIGORILLA_FAN1_PIN
#endif
#define FAN2_PIN         TRIGORILLA_FAN2_PIN
#define ORIG_E0_AUTO_FAN_PIN TRIGORILLA_FAN2_PIN  // Used in Anycubic Kossel example config

#include "pins_RAMPS.h"

#if ENABLED(ANYCUBIC_4MAX)
#ifdef FILWIDTH_PIN
 #undef FILWIDTH_PIN
 #define FILWIDTH_PIN      -1  // Trigorilla have 2 Analog Pins on AUX (D42 & D43!)
#endif                         // ATTENTION: Onboard Description D42 is D43 and describtion D43 ist D42! ).
                               // Set this if you need FILWIDTH. Else -1!

#if HAS_DRIVER(TMC2208)
/**
 * TMC2208 stepper drivers
 *
 * Software serial settings
 *
 * Info: Following Pins can be set for TX: D6, D11, D20, D21, D42, D43
 *                                         ATTENTION: The Onboard PIN Descriptions D42, D43 are wrong (reversed)!
 *                                         Onboard Pin Description D42 is D43 and Pin describtion D43 is D42!
 * Info: Following Pins can be set for RX: D12 ( Additional and alternatively (instead of using the SD-Card): 50, 51, 52, 53.  )
 *                                         You do not need RX Pins. But useful for testing purpose.
 *
 * Info: Recommendation is to use UART RX Pin only for the Extruder E0!
 */

 // Software serial  E0
  #ifdef E0_SERIAL_TX_PIN
   #undef E0_SERIAL_TX_PIN
   #ifdef ANYCUBIC_4MAX_TMC_E0
    #define E0_SERIAL_TX_PIN    11 //SERVO0_PIN
   #else
    #define E0_SERIAL_TX_PIN    -1
   #endif
  #endif
  #ifdef E0_SERIAL_RX_PIN
   #undef E0_SERIAL_RX_PIN
   #ifdef ANYCUBIC_4MAX_TMC_E0
    #define E0_SERIAL_RX_PIN    12 //PS_ON_PIN
   #else
    #define E0_SERIAL_RX_PIN    -1
   #endif
  #endif

 // Software serial  X
  #ifdef X_SERIAL_TX_PIN
   #undef X_SERIAL_TX_PIN
   #define X_SERIAL_TX_PIN    -1
  #endif
  #ifdef X_SERIAL_RX_PIN
   #undef X_SERIAL_RX_PIN
   #define X_SERIAL_RX_PIN    -1
  #endif

  // Software serial  Y
  #ifdef Y_SERIAL_TX_PIN
   #undef Y_SERIAL_TX_PIN
   #define Y_SERIAL_TX_PIN    -1
  #endif
  #ifdef Y_SERIAL_RX_PIN
   #undef Y_SERIAL_RX_PIN
   #define Y_SERIAL_RX_PIN    -1
  #endif

  // Software serial  Z
  #ifdef Z_SERIAL_TX_PIN
   #undef Z_SERIAL_TX_PIN
   #define Z_SERIAL_TX_PIN    -1
  #endif
  #ifdef Z_SERIAL_RX_PIN
   #undef Z_SERIAL_RX_PIN
   #define Z_SERIAL_RX_PIN    -1
  #endif

#endif //TMC2208

#ifdef FIL_RUNOUT_PIN
 #undef FIL_RUNOUT_PIN
 #define FIL_RUNOUT_PIN    2  // Trigorilla Limit Switches are: D19(Z_MAX_PIN), D18(Z_MIN_PIN), D15(Y_MAX_PIN), D14(Y_MIN_PIN), D2(X_MAX_PIN), D3(X_MIN_PIN)
#endif

#ifdef BTN_EN1
  #undef BTN_EN1
  #define BTN_EN1          33  // Anycubic 4MAX specific
#endif

#ifdef BTN_EN2
  #undef BTN_EN2
  #define BTN_EN2          31  // Anycubic 4MAX specific
#endif

#ifdef KILL_PIN
  #undef KILL_PIN
  #define KILL_PIN         -1  // Anycubic 4MAX specific
#endif

#endif // IF ENABLED(ANYCUBIC_4MAX)

//
// AnyCubic made the following changes to 1.1.0-RC8
// If these are appropriate for your LCD let us know.
//
#if 0 && HAS_SPI_LCD

  // LCD Display output pins
  #if BOTH(NEWPANEL, PANEL_ONE)
    #undef LCD_PINS_D6
    #define LCD_PINS_D6    57
  #endif

  // LCD Display input pins
  #if ENABLED(NEWPANEL)
    #if ANY(VIKI2, miniVIKI)
      #undef DOGLCD_A0
      #define DOGLCD_A0    23
    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #undef BEEPER_PIN
      #define BEEPER_PIN   33
      #undef LCD_BACKLIGHT_PIN
      #define LCD_BACKLIGHT_PIN 67
    #endif
  #elif ENABLED(MINIPANEL)
    #undef BEEPER_PIN
    #define BEEPER_PIN     33
    #undef DOGLCD_A0
    #define DOGLCD_A0      42
  #endif

#endif // HAS_SPI_LCD
