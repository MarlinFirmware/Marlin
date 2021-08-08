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
 * Einsy-Rambo pin assignments
 */

#include "env_validate.h"

#define BOARD_INFO_NAME       "Einsy Rambo"
#define DEFAULT_MACHINE_NAME  "Prusa MK3"

//#define MK3_FAN_PINS

//
// TMC2130 Configuration_adv defaults for EinsyRambo
//
#if !AXIS_DRIVER_TYPE_X(TMC2130) || !AXIS_DRIVER_TYPE_Y(TMC2130) || !AXIS_DRIVER_TYPE_Z(TMC2130) || !AXIS_DRIVER_TYPE_E0(TMC2130)
  #error "You must set ([XYZ]|E0)_DRIVER_TYPE to TMC2130 in Configuration.h for EinsyRambo."
#endif

// TMC2130 Diag Pins (currently just for reference)
#define X_DIAG_PIN                            64
#define Y_DIAG_PIN                            69
#define Z_DIAG_PIN                            68
#define E0_DIAG_PIN                           65

//
// Limit Switches
//
// Only use Diag Pins when SENSORLESS_HOMING is enabled for the TMC2130 drivers.
// Otherwise use a physical endstop based configuration.
//
// SERVO0_PIN and Z_MIN_PIN configuration for BLTOUCH sensor when combined with SENSORLESS_HOMING.
//

#if DISABLED(SENSORLESS_HOMING)

  #define X_STOP_PIN                          12
  #define Y_STOP_PIN                          11
  #define Z_STOP_PIN                          10

#else

  #define X_STOP_PIN                  X_DIAG_PIN
  #define Y_STOP_PIN                  Y_DIAG_PIN

  #if ENABLED(BLTOUCH)
    #define Z_STOP_PIN                        11  // Y-MIN
    #define SERVO0_PIN                        10  // Z-MIN
  #else
    #define Z_STOP_PIN                        10
  #endif

#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     10
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      62
#endif

//
// Steppers
//
#define X_STEP_PIN                            37
#define X_DIR_PIN                             49
#define X_ENABLE_PIN                          29
#define X_CS_PIN                              41

#define Y_STEP_PIN                            36
#define Y_DIR_PIN                             48
#define Y_ENABLE_PIN                          28
#define Y_CS_PIN                              39

#define Z_STEP_PIN                            35
#define Z_DIR_PIN                             47
#define Z_ENABLE_PIN                          27
#define Z_CS_PIN                              67

#define E0_STEP_PIN                           34
#define E0_DIR_PIN                            43
#define E0_ENABLE_PIN                         26
#define E0_CS_PIN                             66

//
// Temperature Sensors
//
#define TEMP_0_PIN                             0  // Analog Input, Header J2
#define TEMP_1_PIN                             1  // Analog Input, Header J3
#define TEMP_BOARD_PIN                        91  // Onboard thermistor, 100k TDK NTCG104LH104JT1
#define TEMP_BED_PIN                           2  // Analog Input, Header J6
#define TEMP_PROBE_PIN                         3  // Analog Input, Header J15

//
// Heaters / Fans
//
#define HEATER_0_PIN                           3
#define HEATER_BED_PIN                         4

#ifndef FAN_PIN
  #ifdef MK3_FAN_PINS
    #define FAN_PIN                            6
  #else
    #define FAN_PIN                            8
  #endif
#endif

#ifndef FAN1_PIN
  #ifdef MK3_FAN_PINS
    #define FAN_PIN                           -1
  #else
    #define FAN_PIN                            6
  #endif
#endif

//
// Misc. Functions
//
#define SDSS                                  77
#define LED_PIN                               13

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                       9
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
// use P1 connector for spindle pins
#define SPINDLE_LASER_PWM_PIN                  9  // Hardware PWM
#define SPINDLE_LASER_ENA_PIN                 18  // Pullup!
#define SPINDLE_DIR_PIN                       19

//
// Průša i3 MK2 Multiplexer Support
//
#define E_MUX0_PIN                            17
#define E_MUX1_PIN                            16
#define E_MUX2_PIN                            78  // 84 in MK2 Firmware, with BEEPER as 78

//
// LCD / Controller
//
#if HAS_WIRED_LCD || TOUCH_UI_ULTIPANEL

  #define KILL_PIN                            32

  #if IS_ULTIPANEL || TOUCH_UI_ULTIPANEL

    #if ENABLED(CR10_STOCKDISPLAY)
      #define LCD_PINS_RS                     85
      #define LCD_PINS_ENABLE                 71
      #define LCD_PINS_D4                     70
      #define BTN_EN1                         61
      #define BTN_EN2                         59
    #else
      #define LCD_PINS_RS                     82
      #define LCD_PINS_ENABLE                 61
      #define LCD_PINS_D4                     59
      #define LCD_PINS_D5                     70
      #define LCD_PINS_D6                     85
      #define LCD_PINS_D7                     71
      #define BTN_EN1                         14
      #define BTN_EN2                         72

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

    #define BTN_ENC                            9  // AUX-2
    #define BEEPER_PIN                        84  // AUX-4
    #define SD_DETECT_PIN                     15

  #endif // IS_ULTIPANEL || TOUCH_UI_ULTIPANEL
#endif // HAS_WIRED_LCD

#undef MK3_FAN_PINS
