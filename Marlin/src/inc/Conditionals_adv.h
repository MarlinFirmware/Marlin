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
 * Conditionals_adv.h
 * Defines that depend on advanced configuration.
 */

#if EXTRUDERS == 0
  #define NO_VOLUMETRICS
  #undef TEMP_SENSOR_0
  #undef TEMP_SENSOR_1
  #undef TEMP_SENSOR_2
  #undef TEMP_SENSOR_3
  #undef TEMP_SENSOR_4
  #undef TEMP_SENSOR_5
  #undef TEMP_SENSOR_6
  #undef TEMP_SENSOR_7
  #undef FWRETRACT
  #undef PIDTEMP
  #undef AUTOTEMP
  #undef PID_EXTRUSION_SCALING
  #undef LIN_ADVANCE
  #undef FILAMENT_RUNOUT_SENSOR
  #undef ADVANCED_PAUSE_FEATURE
  #undef FILAMENT_RUNOUT_DISTANCE_MM
  #undef FILAMENT_LOAD_UNLOAD_GCODES
  #undef DISABLE_INACTIVE_EXTRUDER
  #undef FILAMENT_LOAD_UNLOAD_GCODES
  #undef EXTRUDER_RUNOUT_PREVENT
  #undef PREVENT_COLD_EXTRUSION
  #undef PREVENT_LENGTHY_EXTRUDE
  #undef THERMAL_PROTECTION_HOTENDS
  #undef THERMAL_PROTECTION_PERIOD
  #undef WATCH_TEMP_PERIOD
  #undef SHOW_TEMP_ADC_VALUES
#endif

// Multiple Z steppers
#ifndef NUM_Z_STEPPER_DRIVERS
  #define NUM_Z_STEPPER_DRIVERS 1
#endif

#define HAS_CUTTER EITHER(SPINDLE_FEATURE, LASER_FEATURE)

#if !defined(__AVR__) || !defined(USBCON)
  // Define constants and variables for buffering serial data.
  // Use only 0 or powers of 2 greater than 1
  // : [0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, ...]
  #ifndef RX_BUFFER_SIZE
    #define RX_BUFFER_SIZE 128
  #endif
  // 256 is the max TX buffer limit due to uint8_t head and tail
  // : [0, 4, 8, 16, 32, 64, 128, 256]
  #ifndef TX_BUFFER_SIZE
    #define TX_BUFFER_SIZE 32
  #endif
#else
  // SERIAL_XON_XOFF not supported on USB-native devices
  #undef SERIAL_XON_XOFF
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #ifndef ACTION_ON_PAUSE
    #define ACTION_ON_PAUSE   "pause"
  #endif
  #ifndef ACTION_ON_PAUSED
    #define ACTION_ON_PAUSED  "paused"
  #endif
  #ifndef ACTION_ON_RESUME
    #define ACTION_ON_RESUME  "resume"
  #endif
  #ifndef ACTION_ON_RESUMED
    #define ACTION_ON_RESUMED "resumed"
  #endif
  #ifndef ACTION_ON_CANCEL
    #define ACTION_ON_CANCEL  "cancel"
  #endif
  #ifndef ACTION_ON_KILL
    #define ACTION_ON_KILL    "poweroff"
  #endif
  #if HAS_FILAMENT_SENSOR
    #ifndef ACTION_ON_FILAMENT_RUNOUT
      #define ACTION_ON_FILAMENT_RUNOUT "filament_runout"
    #endif
    #ifndef ACTION_REASON_ON_FILAMENT_RUNOUT
      #define ACTION_REASON_ON_FILAMENT_RUNOUT "filament_runout"
    #endif
  #endif
  #if ENABLED(G29_RETRY_AND_RECOVER)
    #ifndef ACTION_ON_G29_RECOVER
      #define ACTION_ON_G29_RECOVER "probe_rewipe"
    #endif
    #ifndef ACTION_ON_G29_FAILURE
      #define ACTION_ON_G29_FAILURE "probe_failed"
    #endif
  #endif
#endif

#if ENABLED(FYSETC_MINI_12864_2_1)
  #define LED_CONTROL_MENU
  #define LED_USER_PRESET_STARTUP
  #define LED_COLOR_PRESETS
  #ifndef LED_USER_PRESET_RED
    #define LED_USER_PRESET_RED        255
  #endif
  #ifndef LED_USER_PRESET_GREEN
    #define LED_USER_PRESET_GREEN      128
  #endif
  #ifndef LED_USER_PRESET_BLUE
    #define LED_USER_PRESET_BLUE         0
  #endif
  #ifndef LED_USER_PRESET_BRIGHTNESS
    #define LED_USER_PRESET_BRIGHTNESS 255
  #endif
#endif

// Extensible UI pin mapping for RepRapDiscount
#define TOUCH_UI_ULTIPANEL ENABLED(TOUCH_UI_FTDI_EVE) && ANY(AO_EXP1_PINMAP, AO_EXP2_PINMAP, CR10_TFT_PINMAP)

// Poll-based jogging for joystick and other devices
#if ENABLED(JOYSTICK)
  #define POLL_JOG
#endif

// G60/G61 Position Save
#if SAVED_POSITIONS > 256
  #error "SAVED_POSITIONS must be an integer from 0 to 256."
#endif
