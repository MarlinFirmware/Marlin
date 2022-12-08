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
  #define SERVO0_PIN                          PinE3
  #define SERVO1_PIN                          PinG5
  #define SERVO2_PIN                          PinB5
  #define SERVO3_PIN                          PinH3
#endif

//
// PWM FETS
//
#define MOSFET_B_PIN                          PinL4  // HEATER1

//
// Heaters / Fans
//
#define FAN_PIN                               PinH6  // FAN0
#define FAN1_PIN                              PinH4  // FAN1
#define FAN2_PIN                              PinL5  // FAN2
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN               FAN2_PIN
#endif

//
// AnyCubic pin mappings
//
// Define the appropriate mapping option in Configuration.h:
// - TRIGORILLA_MAPPING_CHIRON
// - TRIGORILLA_MAPPING_I3MEGA
//

//
// Limit Switches
//
//#define ANYCUBIC_4_MAX_PRO_ENDSTOPS
#if ENABLED(ANYCUBIC_4_MAX_PRO_ENDSTOPS)
  #define X_MAX_PIN                           PinL6
  #define Y_STOP_PIN                          PinD2
#elif EITHER(TRIGORILLA_MAPPING_CHIRON, TRIGORILLA_MAPPING_I3MEGA)
  // Chiron uses AUX header for Y and Z endstops
  #define Y_STOP_PIN                          PinL7  // AUX
  #define Z_STOP_PIN                          PinL6  // AUX
  #define Z2_MIN_PIN                          PinD3  // Z-

  #ifndef Z_MIN_PROBE_PIN
    #define Z_MIN_PROBE_PIN                   PinE4
  #endif

  #define CONTROLLER_FAN_PIN            FAN1_PIN

  #if ENABLED(POWER_LOSS_RECOVERY)
    #define OUTAGETEST_PIN                    PinE6
    #define OUTAGECON_PIN                     PinF4
  #endif

  #if ENABLED(TRIGORILLA_MAPPING_CHIRON)
    #ifndef FIL_RUNOUT_PIN
      #define FIL_RUNOUT_PIN                  PinC4
    #endif

    // Chiron swaps the Z stepper connections
    #define Z_STEP_PIN                        PinC1
    #define Z_DIR_PIN                         PinC3
    #define Z_ENABLE_PIN                      PinC7
    #define Z_CS_PIN                          PinL5

    #define Z2_STEP_PIN                       PinL3
    #define Z2_DIR_PIN                        PinL1
    #define Z2_ENABLE_PIN                     PinK0
    #define Z2_CS_PIN                         PinG1

    #define HEATER_BED_PIN          MOSFET_B_PIN  // HEATER1
  #else
    #ifndef FIL_RUNOUT_PIN
      #define FIL_RUNOUT_PIN                  PinD2
    #endif
  #endif
#endif

#if EITHER(ANYCUBIC_LCD_CHIRON, ANYCUBIC_LCD_I3MEGA)
  #define BEEPER_PIN                          PinC6
  #define SD_DETECT_PIN                       PinL0
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

//
// AnyCubic made the following changes to 1.1.0-RC8
// If these are appropriate for your LCD let us know.
//
#if 0 && HAS_WIRED_LCD

  // LCD Display output pins
  #if BOTH(IS_NEWPANEL, PANEL_ONE)
    #undef LCD_PINS_D6
    #define LCD_PINS_D6                       PinF3
  #endif

  // LCD Display input pins
  #if IS_NEWPANEL
    #if EITHER(VIKI2, miniVIKI)
      #undef DOGLCD_A0
      #define DOGLCD_A0                       PinA1
    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #undef BEEPER_PIN
      #define BEEPER_PIN                      PinC4
      #undef LCD_BACKLIGHT_PIN
      #define LCD_BACKLIGHT_PIN               PinK5
    #endif
  #elif ENABLED(MINIPANEL)
    #undef BEEPER_PIN
    #define BEEPER_PIN                        PinC4
    #undef DOGLCD_A0
    #define DOGLCD_A0                         PinL7
  #endif

#endif // HAS_WIRED_LCD
