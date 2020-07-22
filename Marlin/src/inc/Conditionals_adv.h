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

#if ENABLED(MIXING_EXTRUDER) && (ENABLED(RETRACT_SYNC_MIXING) || BOTH(FILAMENT_LOAD_UNLOAD_GCODES, FILAMENT_UNLOAD_ALL_EXTRUDERS))
  #define HAS_MIXER_SYNC_CHANNEL 1
#endif

#if EITHER(DUAL_X_CARRIAGE, MULTI_NOZZLE_DUPLICATION)
  #define HAS_DUPLICATION_MODE 1
#endif

#if ENABLED(PRINTCOUNTER) && (SERVICE_INTERVAL_1 > 0 || SERVICE_INTERVAL_2 > 0 || SERVICE_INTERVAL_3 > 0)
  #define HAS_SERVICE_INTERVALS 1
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #define HAS_FILAMENT_SENSOR 1
  #ifdef FILAMENT_RUNOUT_DISTANCE_MM
    #define HAS_FILAMENT_RUNOUT_DISTANCE 1
  #endif
#endif

// Let SD_FINISHED_RELEASECOMMAND stand in for SD_FINISHED_STEPPERRELEASE
#if ENABLED(SD_FINISHED_STEPPERRELEASE)
  #ifndef SD_FINISHED_RELEASECOMMAND
    #define SD_FINISHED_RELEASECOMMAND "M84" // planner.finish_and_disable()
  #endif
#else
  #undef SD_FINISHED_RELEASECOMMAND
#endif

#if EITHER(SDSUPPORT, LCD_SET_PROGRESS_MANUALLY)
  #define HAS_PRINT_PROGRESS 1
#endif

#if HAS_PRINT_PROGRESS && EITHER(PRINT_PROGRESS_SHOW_DECIMALS, SHOW_REMAINING_TIME)
  #define HAS_PRINT_PROGRESS_PERMYRIAD 1
#endif

#if ANY(MARLIN_BRICKOUT, MARLIN_INVADERS, MARLIN_SNAKE, MARLIN_MAZE)
  #define HAS_GAMES 1
  #if (1 < ENABLED(MARLIN_BRICKOUT) + ENABLED(MARLIN_INVADERS) + ENABLED(MARLIN_SNAKE) + ENABLED(MARLIN_MAZE))
    #define HAS_GAME_MENU 1
  #endif
#endif

#if ANY(FWRETRACT, HAS_LEVELING, SKEW_CORRECTION)
  #define HAS_POSITION_MODIFIERS 1
#endif

#if ANY(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS, Z_MULTI_ENDSTOPS)
  #define HAS_EXTRA_ENDSTOPS 1
#endif
#if EITHER(MIN_SOFTWARE_ENDSTOPS, MAX_SOFTWARE_ENDSTOPS)
  #define HAS_SOFTWARE_ENDSTOPS 1
#endif
#if ANY(EXTENSIBLE_UI, NEWPANEL, EMERGENCY_PARSER, HAS_ADC_BUTTONS, DWIN_CREALITY_LCD)
  #define HAS_RESUME_CONTINUE 1
#endif

#if ANY(BLINKM, RGB_LED, RGBW_LED, PCA9632, PCA9533, NEOPIXEL_LED)
  #define HAS_COLOR_LEDS 1
#endif
#if ALL(HAS_RESUME_CONTINUE, PRINTER_EVENT_LEDS, SDSUPPORT)
  #define HAS_LEDS_OFF_FLAG 1
#endif

#if EITHER(DIGIPOT_MCP4018, DIGIPOT_MCP4451)
  #define HAS_I2C_DIGIPOT 1
#endif

// Multiple Z steppers
#ifndef NUM_Z_STEPPER_DRIVERS
  #define NUM_Z_STEPPER_DRIVERS 1
#endif

#if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
  #undef Z_STEPPER_ALIGN_AMP
#endif
#ifndef Z_STEPPER_ALIGN_AMP
  #define Z_STEPPER_ALIGN_AMP 1.0
#endif

//
// Spindle/Laser power display types
// Defined here so sanity checks can use them
//
#if EITHER(SPINDLE_FEATURE, LASER_FEATURE)
  #define HAS_CUTTER 1
  #define _CUTTER_POWER_PWM255  1
  #define _CUTTER_POWER_PERCENT 2
  #define _CUTTER_POWER_RPM     3
  #define _CUTTER_POWER(V)      _CAT(_CUTTER_POWER_, V)
  #define CUTTER_UNIT_IS(V)    (_CUTTER_POWER(CUTTER_POWER_UNIT)    == _CUTTER_POWER(V))
#endif

// Add features that need hardware PWM here
#if ANY(FAST_PWM_FAN, SPINDLE_LASER_PWM)
  #define NEEDS_HARDWARE_PWM 1
#endif

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

#if ENABLED(FYSETC_MINI_12864_2_1, FYSETC_242_OLED_12864)
  #define LED_CONTROL_MENU
  #define LED_USER_PRESET_STARTUP
  #define LED_COLOR_PRESETS
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

// Set defaults for unspecified LED user colors
#if ENABLED(LED_CONTROL_MENU)
  #ifndef LED_USER_PRESET_RED
    #define LED_USER_PRESET_RED       255
  #endif
  #ifndef LED_USER_PRESET_GREEN
    #define LED_USER_PRESET_GREEN     255
  #endif
  #ifndef LED_USER_PRESET_BLUE
    #define LED_USER_PRESET_BLUE      255
  #endif
  #ifndef LED_USER_PRESET_WHITE
    #define LED_USER_PRESET_WHITE     0
  #endif
  #ifndef LED_USER_PRESET_BRIGHTNESS
    #ifdef NEOPIXEL_BRIGHTNESS
      #define LED_USER_PRESET_BRIGHTNESS NEOPIXEL_BRIGHTNESS
    #else
      #define LED_USER_PRESET_BRIGHTNESS 255
    #endif
  #endif
#endif

// If platform requires early initialization of watchdog to properly boot
#if ENABLED(USE_WATCHDOG) && defined(ARDUINO_ARCH_SAM)
  #define EARLY_WATCHDOG 1
#endif

// Extensible UI pin mapping for RepRapDiscount
#if ENABLED(TOUCH_UI_FTDI_EVE) && ANY(AO_EXP1_PINMAP, AO_EXP2_PINMAP, CR10_TFT_PINMAP)
  #define TOUCH_UI_ULTIPANEL 1
#endif

// Poll-based jogging for joystick and other devices
#if ENABLED(JOYSTICK)
  #define POLL_JOG
#endif

/**
 * Driver Timings (in nanoseconds)
 * NOTE: Driver timing order is longest-to-shortest duration.
 *       Preserve this ordering when adding new drivers.
 */
#ifndef MINIMUM_STEPPER_POST_DIR_DELAY
  #if HAS_DRIVER(TB6560)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 15000
  #elif HAS_DRIVER(TB6600)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 1500
  #elif HAS_DRIVER(DRV8825)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 650
  #elif HAS_DRIVER(LV8729)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 500
  #elif HAS_DRIVER(A5984)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 400
  #elif HAS_DRIVER(A4988)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 200
  #elif HAS_TRINAMIC_CONFIG || HAS_TRINAMIC_STANDALONE
    #define MINIMUM_STEPPER_POST_DIR_DELAY 60
  #else
    #define MINIMUM_STEPPER_POST_DIR_DELAY 0   // Expect at least 10µS since one Stepper ISR must transpire
  #endif
#endif

#ifndef MINIMUM_STEPPER_PRE_DIR_DELAY
  #define MINIMUM_STEPPER_PRE_DIR_DELAY MINIMUM_STEPPER_POST_DIR_DELAY
#endif

#ifndef MINIMUM_STEPPER_PULSE
  #if HAS_DRIVER(TB6560)
    #define MINIMUM_STEPPER_PULSE 30
  #elif HAS_DRIVER(TB6600)
    #define MINIMUM_STEPPER_PULSE 3
  #elif HAS_DRIVER(DRV8825)
    #define MINIMUM_STEPPER_PULSE 2
  #elif HAS_DRIVER(A4988) || HAS_DRIVER(A5984)
    #define MINIMUM_STEPPER_PULSE 1
  #elif HAS_TRINAMIC_CONFIG || HAS_TRINAMIC_STANDALONE
    #define MINIMUM_STEPPER_PULSE 0
  #elif HAS_DRIVER(LV8729)
    #define MINIMUM_STEPPER_PULSE 0
  #else
    #define MINIMUM_STEPPER_PULSE 2
  #endif
#endif

#ifndef MAXIMUM_STEPPER_RATE
  #if HAS_DRIVER(TB6560)
    #define MAXIMUM_STEPPER_RATE 15000
  #elif HAS_DRIVER(TB6600)
    #define MAXIMUM_STEPPER_RATE 150000
  #elif HAS_DRIVER(DRV8825)
    #define MAXIMUM_STEPPER_RATE 250000
  #elif HAS_DRIVER(A4988)
    #define MAXIMUM_STEPPER_RATE 500000
  #elif HAS_DRIVER(LV8729)
    #define MAXIMUM_STEPPER_RATE 1000000
  #elif HAS_TRINAMIC_CONFIG || HAS_TRINAMIC_STANDALONE
    #define MAXIMUM_STEPPER_RATE 5000000
  #else
    #define MAXIMUM_STEPPER_RATE 250000
  #endif
#endif

#if ENABLED(DIRECT_STEPPING)
  #ifndef STEPPER_PAGES
    #define STEPPER_PAGES 16
  #endif
  #ifndef STEPPER_PAGE_FORMAT
    #define STEPPER_PAGE_FORMAT SP_4x2_256
  #endif
  #ifndef PAGE_MANAGER
    #define PAGE_MANAGER SerialPageManager
  #endif
#endif

//
// SD Card connection methods
// Defined here so pins and sanity checks can use them
//
#if ENABLED(SDSUPPORT)
  #define _SDCARD_LCD          1
  #define _SDCARD_ONBOARD      2
  #define _SDCARD_CUSTOM_CABLE 3
  #define _SDCARD_ID(V) _CAT(_SDCARD_, V)
  #define SD_CONNECTION_IS(V) (_SDCARD_ID(SDCARD_CONNECTION) == _SDCARD_ID(V))
#else
  #define SD_CONNECTION_IS(...) 0
#endif

// Power Monitor sensors
#if EITHER(POWER_MONITOR_CURRENT, POWER_MONITOR_VOLTAGE)
  #define HAS_POWER_MONITOR 1
#endif
#if ENABLED(POWER_MONITOR_CURRENT) && defined(POWER_MONITOR_FIXED_VOLTAGE)
  #define HAS_POWER_MONITOR_VREF 1
#endif
#if BOTH(HAS_POWER_MONITOR_VREF, POWER_MONITOR_CURRENT)
  #define HAS_POWER_MONITOR_WATTS 1
#endif

// Flag if an EEPROM type is pre-selected
#if ENABLED(EEPROM_SETTINGS) && NONE(I2C_EEPROM, SPI_EEPROM, QSPI_EEPROM, FLASH_EEPROM_EMULATION, SRAM_EEPROM_EMULATION, SDCARD_EEPROM_EMULATION)
  #define NO_EEPROM_SELECTED 1
#endif
