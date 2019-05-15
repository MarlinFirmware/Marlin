/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 *  Z-Bolt X Series board – based on Arduino Mega2560 
 */

#if HOTENDS > 4 || E_STEPPERS > 4
  #error "Z-Bolt X Series board supports up to 4 hotends / E-steppers."
#endif

#define BOARD_NAME "Z-Bolt X Series"



#if ENABLED(TARGET_LPC1768)
  #error "Oops! Set MOTHERBOARD to an LPC1768-based board when building for LPC1768."
#elif defined(__STM32F1__)
  #error "Oops! Set MOTHERBOARD to an STM32F1-based board when building for STM32F1."
#endif

#if DISABLED(IS_RAMPS_SMART, IS_RAMPS_DUO, IS_RAMPS4DUE, TARGET_LPC1768)
  #if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
    #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
  #endif
#endif


//
// Servos
//
#ifndef SERVO0_PIN
    #define SERVO0_PIN     11
#endif
// #define SERVO1_PIN          6
// #define SERVO2_PIN          5
#ifndef SERVO3_PIN
  #define SERVO3_PIN        4
#endif

//
// Limit Switches
//
#define X_MIN_PIN           3
#ifndef X_MAX_PIN
  #define X_MAX_PIN         2
#endif
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  32
#endif

//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#ifndef X_CS_PIN
  #define X_CS_PIN         -1
#endif

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#ifndef Y_CS_PIN
  #define Y_CS_PIN         -1
#endif

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#ifndef Z_CS_PIN
  #define Z_CS_PIN         -1
#endif

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#ifndef E0_CS_PIN
  #define E0_CS_PIN        -1
#endif

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
#ifndef E1_CS_PIN
  #define E1_CS_PIN        -1
#endif

// Red
#define E2_STEP_PIN        42
#define E2_DIR_PIN         40
#define E2_ENABLE_PIN      65
#ifndef E2_CS_PIN
  #define E2_CS_PIN        -1
#endif

// Black
#define E3_STEP_PIN        44
#define E3_DIR_PIN         64
#define E3_ENABLE_PIN      66
#ifndef E3_CS_PIN
  #define E3_CS_PIN        -1
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         13   // Analog Input
#define TEMP_1_PIN         15   // Analog Input
#define TEMP_2_PIN          5   // Analog Input (BLACK)
#define TEMP_3_PIN          9   // Analog Input (RED)

#define TEMP_BED_PIN       14   // Analog Input

// // SPI for Max6675 or Max31855 Thermocouple
// #if DISABLED(SDSUPPORT)
//   #define MAX6675_SS_PIN   66   // Do not use pin 53 if there is even the remote possibility of using Display/SD card
// #else
//   #define MAX6675_SS_PIN   66   // Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
// #endif

//
// Augmentation for auto-assigning RAMPS plugs
//
// #if DISABLED(IS_RAMPS_EEB, IS_RAMPS_EEF, IS_RAMPS_EFB, IS_RAMPS_EFF, IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
//   #if HOTENDS > 1
//     #if TEMP_SENSOR_BED
//       #define IS_RAMPS_EEB
//     #else
//       #define IS_RAMPS_EEF
//     #endif
//   #elif TEMP_SENSOR_BED
//     #define IS_RAMPS_EFB
//   #else
//     #define IS_RAMPS_EFF
//   #endif
// #endif

//
// Heaters / Fans
//
// #ifndef MOSFET_D_PIN
//   #define MOSFET_D_PIN     -1
// #endif

#define HEATER_0_PIN     10
#define HEATER_1_PIN      7
#define HEATER_2_PIN      6
#define HEATER_3_PIN      5

#define HEATER_BED_PIN   8

// #if ENABLED(IS_RAMPS_EFB)                      // Hotend, Fan, Bed
//   #define HEATER_BED_PIN   RAMPS_D8_PIN
// #elif ENABLED(IS_RAMPS_EEF)                    // Hotend, Hotend, Fan
//   #define HEATER_1_PIN     RAMPS_D9_PIN
// #elif ENABLED(IS_RAMPS_EEB)                    // Hotend, Hotend, Bed
//   #define HEATER_1_PIN     RAMPS_D9_PIN
//   #define HEATER_BED_PIN   RAMPS_D8_PIN
// #elif ENABLED(IS_RAMPS_EFF)                    // Hotend, Fan, Fan
//   #define FAN1_PIN         RAMPS_D8_PIN
// #elif DISABLED(IS_RAMPS_SF)                    // Not Spindle, Fan (i.e., "EFBF" or "EFBE")
//   #define HEATER_BED_PIN   RAMPS_D8_PIN
//   #if HOTENDS == 1
//     #define FAN1_PIN       MOSFET_D_PIN
//   #else
//     #define HEATER_1_PIN   MOSFET_D_PIN
//     #define HEATER_2_PIN   MOSFET_D_PIN
//   #endif
// #endif



// #ifndef FAN_PIN
//   #if EITHER(IS_RAMPS_EFB, IS_RAMPS_EFF)          // Hotend, Fan, Bed or Hotend, Fan, Fan
//     #define FAN_PIN        RAMPS_D9_PIN
//   #elif EITHER(IS_RAMPS_EEF, IS_RAMPS_SF)         // Hotend, Hotend, Fan or Spindle, Fan
//     #define FAN_PIN        RAMPS_D8_PIN
//   #elif ENABLED(IS_RAMPS_EEB)                  // Hotend, Hotend, Bed
//     #define FAN_PIN         4                  // IO pin. Buffer needed
//   #else                                        // Non-specific are "EFB" (i.e., "EFBF" or "EFBE")

//   #endif
// #endif

#define FAN_PIN             9
//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN      5   // Analog Input on AUX2
#endif

// RAMPS 1.4 DIO 4 on the servos connector
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN    4
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN        12
#endif

#define AUX2_PINS_FREE !( BOTH(ULTRA_LCD, NEWPANEL) && ANY(PANEL_ONE, VIKI2, miniVIKI, MINIPANEL, REPRAPWORLD_KEYPAD) )

#if ENABLED(CASE_LIGHT_ENABLE) && !defined(CASE_LIGHT_PIN) && !defined(SPINDLE_LASER_ENA_PIN)
  #if NUM_SERVOS <= 1 // try to use servo connector first
    #define CASE_LIGHT_PIN    6   // MUST BE HARDWARE PWM
  #elif AUX2_PINS_FREE
    #define CASE_LIGHT_PIN   44   // MUST BE HARDWARE PWM
  #endif
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if ENABLED(SPINDLE_LASER_ENABLE) && !PIN_EXISTS(SPINDLE_LASER_ENA)
  #if !defined(NUM_SERVOS) || NUM_SERVOS == 0 // try to use servo connector first
    #define SPINDLE_LASER_ENA_PIN     4   // Pin should have a pullup/pulldown!
    #define SPINDLE_LASER_PWM_PIN     6   // MUST BE HARDWARE PWM
    #define SPINDLE_DIR_PIN           5
  #elif AUX2_PINS_FREE
    #define SPINDLE_LASER_ENA_PIN    40   // Pin should have a pullup/pulldown!
    #define SPINDLE_LASER_PWM_PIN    44   // MUST BE HARDWARE PWM
    #define SPINDLE_DIR_PIN          65
  #endif
#endif

#undef AUX2_PINS_FREE

//
// TMC software SPI
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    66
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    44
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     64
  #endif
#endif

#if HAS_DRIVER(TMC2208)
  /**
   * TMC2208 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //

  #define X_SERIAL_TX_PIN    40
  #define X_SERIAL_RX_PIN    63
  #define X2_SERIAL_TX_PIN   -1
  #define X2_SERIAL_RX_PIN   -1

  #define Y_SERIAL_TX_PIN    59
  #define Y_SERIAL_RX_PIN    64
  #define Y2_SERIAL_TX_PIN   -1
  #define Y2_SERIAL_RX_PIN   -1

  #define Z_SERIAL_TX_PIN    42
  #define Z_SERIAL_RX_PIN    65
  #define Z2_SERIAL_TX_PIN   -1
  #define Z2_SERIAL_RX_PIN   -1

  #define E0_SERIAL_TX_PIN   44
  #define E0_SERIAL_RX_PIN   66
  #define E1_SERIAL_TX_PIN   -1
  #define E1_SERIAL_RX_PIN   -1
  #define E2_SERIAL_TX_PIN   -1
  #define E2_SERIAL_RX_PIN   -1
  #define E3_SERIAL_TX_PIN   -1
  #define E3_SERIAL_RX_PIN   -1
  #define E4_SERIAL_TX_PIN   -1
  #define E4_SERIAL_RX_PIN   -1
#endif






