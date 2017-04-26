/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef PINS_H
#define PINS_H

#if MB(GEN7_CUSTOM)
  #include "pins_GEN7_CUSTOM.h"
#elif MB(GEN7_12)
  #include "pins_GEN7_12.h"
#elif MB(GEN7_13)
  #include "pins_GEN7_13.h"
#elif MB(GEN7_14)
  #include "pins_GEN7_14.h"
#elif MB(CNCONTROLS_11)
  #include "pins_CNCONTROLS_11.h"
#elif MB(CNCONTROLS_12)
  #include "pins_CNCONTROLS_12.h"
#elif MB(CHEAPTRONIC)
  #include "pins_CHEAPTRONIC.h"
#elif MB(SETHI)
  #include "pins_SETHI.h"
#elif MB(MIGHTYBOARD_REVE)
  #include "pins_MIGHTYBOARD_REVE.h"
#elif MB(RAMPS_OLD)
  #include "pins_RAMPS_OLD.h"
#elif MB(RAMPS_13_EFB)
  #define IS_RAMPS_EFB
  #include "pins_RAMPS_13.h"
#elif MB(RAMPS_13_EEB)
  #define IS_RAMPS_EEB
  #include "pins_RAMPS_13.h"
#elif MB(RAMPS_13_EFF)
  #define IS_RAMPS_EFF
  #include "pins_RAMPS_13.h"
#elif MB(RAMPS_13_EEF)
  #define IS_RAMPS_EEF
  #include "pins_RAMPS_13.h"
#elif MB(RAMPS_13_SF)
  #define IS_RAMPS_SF
  #include "pins_RAMPS_13.h"
#elif MB(RAMPS_14_EFB)
  #define IS_RAMPS_EFB
  #include "pins_RAMPS.h"
#elif MB(RAMPS_14_EEB)
  #define IS_RAMPS_EEB
  #include "pins_RAMPS.h"
#elif MB(RAMPS_14_EFF)
  #define IS_RAMPS_EFF
  #include "pins_RAMPS.h"
#elif MB(RAMPS_14_EEF)
  #define IS_RAMPS_EEF
  #include "pins_RAMPS.h"
#elif MB(RAMPS_14_SF)
  #define IS_RAMPS_SF
  #include "pins_RAMPS.h"
#elif MB(GEN6)
  #include "pins_GEN6.h"
#elif MB(GEN6_DELUXE)
  #include "pins_GEN6_DELUXE.h"
#elif MB(SANGUINOLOLU_11)
  #include "pins_SANGUINOLOLU_11.h"
#elif MB(SANGUINOLOLU_12)
  #include "pins_SANGUINOLOLU_12.h"
#elif MB(MELZI)
  #include "pins_MELZI.h"
#elif MB(STB_11)
  #include "pins_STB_11.h"
#elif MB(AZTEEG_X1)
  #include "pins_AZTEEG_X1.h"
#elif MB(MELZI_MAKR3D)
  #include "pins_MELZI_MAKR3D.h"
#elif MB(AZTEEG_X3)
  #include "pins_AZTEEG_X3.h"
#elif MB(AZTEEG_X3_PRO)
  #include "pins_AZTEEG_X3_PRO.h"
#elif MB(ULTIMAKER)
  #include "pins_ULTIMAKER.h"
#elif MB(ULTIMAKER_OLD)
  #include "pins_ULTIMAKER_OLD.h"
#elif MB(ULTIMAIN_2)
  #include "pins_ULTIMAIN_2.h"
#elif MB(3DRAG)
  #include "pins_3DRAG.h"
#elif MB(K8200)
  #include "pins_K8200.h"
#elif MB(K8400)
  #include "pins_K8400.h"
#elif MB(TEENSYLU)
  #include "pins_TEENSYLU.h"
#elif MB(RUMBA)
  #include "pins_RUMBA.h"
#elif MB(PRINTRBOARD)
  #include "pins_PRINTRBOARD.h"
#elif MB(PRINTRBOARD_REVF)
  #include "pins_PRINTRBOARD_REVF.h"
#elif MB(BRAINWAVE)
  #include "pins_BRAINWAVE.h"
#elif MB(BRAINWAVE_PRO)
  #include "pins_BRAINWAVE_PRO.h"
#elif MB(SAV_MKI)
  #include "pins_SAV_MKI.h"
#elif MB(TEENSY2)
  #include "pins_TEENSY2.h"
#elif MB(GEN3_PLUS)
  #include "pins_GEN3_PLUS.h"
#elif MB(GEN3_MONOLITHIC)
  #include "pins_GEN3_MONOLITHIC.h"
#elif MB(MEGATRONICS)
  #include "pins_MEGATRONICS.h"
#elif MB(MINITRONICS)
  #include "pins_MINITRONICS.h"
#elif MB(MEGATRONICS_2)
  #include "pins_MEGATRONICS_2.h"
#elif MB(MEGATRONICS_3)
  #include "pins_MEGATRONICS_3.h"
#elif MB(MEGATRONICS_31)
  #define MEGATRONICS_31
  #include "pins_MEGATRONICS_3.h" 
#elif MB(OMCA_A)
  #include "pins_OMCA_A.h"
#elif MB(OMCA)
  #include "pins_OMCA.h"
#elif MB(RAMBO)
  #include "pins_RAMBO.h"
#elif MB(MINIRAMBO)
  #include "pins_MINIRAMBO.h"
#elif MB(ELEFU_3)
  #include "pins_ELEFU_3.h"
#elif MB(5DPRINT)
  #include "pins_5DPRINT.h"
#elif MB(LEAPFROG)
  #include "pins_LEAPFROG.h"
#elif MB(BAM_DICE)
  #include "pins_RAMPS.h"
#elif MB(BAM_DICE_DUE)
  #include "pins_BAM_DICE_DUE.h"
#elif MB(FELIX2)
  #include "pins_FELIX2.h"
#elif MB(MKS_BASE)
  #include "pins_MKS_BASE.h"
#elif MB(RIGIDBOARD)
  #include "pins_RIGIDBOARD.h"
#elif MB(RIGIDBOARD_V2)
  #include "pins_RIGIDBOARD_V2.h"
#elif MB(MEGACONTROLLER)
  #include "pins_MEGACONTROLLER.h"
#elif MB(BQ_ZUM_MEGA_3D)
  #include "pins_BQ_ZUM_MEGA_3D.h"
#elif MB(AJ4P)
  #include "pins_AJ4P.h"
#elif MB(MKS_13)
  #include "pins_MKS_13.h"
#elif MB(SAINSMART_2IN1)
  #include "pins_SAINSMART_2IN1.h"
#else
  #error "Unknown MOTHERBOARD value set in Configuration.h"
#endif

// Define certain undefined pins
#ifndef X_MS1_PIN
  #define X_MS1_PIN -1
#endif
#ifndef X_MS2_PIN
  #define X_MS2_PIN -1
#endif
#ifndef Y_MS1_PIN
  #define Y_MS1_PIN -1
#endif
#ifndef Y_MS2_PIN
  #define Y_MS2_PIN -1
#endif
#ifndef Z_MS1_PIN
  #define Z_MS1_PIN -1
#endif
#ifndef Z_MS2_PIN
  #define Z_MS2_PIN -1
#endif
#ifndef E0_MS1_PIN
  #define E0_MS1_PIN -1
#endif
#ifndef E0_MS2_PIN
  #define E0_MS2_PIN -1
#endif
#ifndef E1_MS1_PIN
  #define E1_MS1_PIN -1
#endif
#ifndef E1_MS2_PIN
  #define E1_MS2_PIN -1
#endif

#ifndef FAN_PIN
  #define FAN_PIN -1
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN -1
#endif
#ifndef FAN2_PIN
  #define FAN2_PIN -1
#endif
#ifndef CONTROLLERFAN_PIN
  #define CONTROLLERFAN_PIN  -1
#endif

#ifndef HEATER_0_PIN
  #define HEATER_0_PIN -1
#endif
#ifndef HEATER_1_PIN
  #define HEATER_1_PIN -1
#endif
#ifndef HEATER_2_PIN
  #define HEATER_2_PIN -1
#endif
#ifndef HEATER_3_PIN
  #define HEATER_3_PIN -1
#endif
#ifndef HEATER_4_PIN
  #define HEATER_4_PIN -1
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN -1
#endif

#ifndef TEMP_0_PIN
  #define TEMP_0_PIN -1
#endif
#ifndef TEMP_1_PIN
  #define TEMP_1_PIN -1
#endif
#ifndef TEMP_2_PIN
  #define TEMP_2_PIN -1
#endif
#ifndef TEMP_3_PIN
  #define TEMP_3_PIN -1
#endif
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN -1
#endif

#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN -1
#endif
#ifndef SDPOWER
  #define SDPOWER -1
#endif
#ifndef SDSS
  #define SDSS -1
#endif
#ifndef LED_PIN
  #define LED_PIN -1
#endif
#ifndef PS_ON_PIN
  #define PS_ON_PIN -1
#endif
#ifndef KILL_PIN
  #define KILL_PIN -1
#endif
#ifndef SUICIDE_PIN
  #define SUICIDE_PIN -1
#endif

#ifndef MAX_EXTRUDERS
  #define MAX_EXTRUDERS 4
#endif

// Marlin needs to account for pins that equal -1
#define marlinAnalogInputToDigitalPin(p) ((p) == -1 ? -1 : analogInputToDigitalPin(p))

//
// Assign auto fan pins if needed
//
#if !defined(E0_AUTO_FAN_PIN) && defined(ORIG_E0_AUTO_FAN_PIN)
  #define E0_AUTO_FAN_PIN ORIG_E0_AUTO_FAN_PIN
#endif
#if !defined(E1_AUTO_FAN_PIN) && defined(ORIG_E1_AUTO_FAN_PIN)
  #define E1_AUTO_FAN_PIN ORIG_E1_AUTO_FAN_PIN
#endif
#if !defined(E2_AUTO_FAN_PIN) && defined(ORIG_E2_AUTO_FAN_PIN)
  #define E2_AUTO_FAN_PIN ORIG_E2_AUTO_FAN_PIN
#endif
#if !defined(E3_AUTO_FAN_PIN) && defined(ORIG_E3_AUTO_FAN_PIN)
  #define E3_AUTO_FAN_PIN ORIG_E3_AUTO_FAN_PIN
#endif

// List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, E0_MS1_PIN, E0_MS2_PIN,
#define _E1_PINS
#define _E2_PINS
#define _E3_PINS
#define _E4_PINS

#if EXTRUDERS > 1
  #undef _E1_PINS
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, E1_MS1_PIN, E1_MS2_PIN,
  #if EXTRUDERS > 2
    #undef _E2_PINS
    #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN,
    #if EXTRUDERS > 3
      #undef _E3_PINS
      #define _E3_PINS E3_STEP_PIN, E3_DIR_PIN, E3_ENABLE_PIN,
      #if EXTRUDERS > 4
        #undef _E4_PINS
        #define _E4_PINS E4_STEP_PIN, E4_DIR_PIN, E4_ENABLE_PIN,
      #endif
    #endif
  #endif
#endif

#define _H0_PINS HEATER_0_PIN, E0_AUTO_FAN_PIN, marlinAnalogInputToDigitalPin(TEMP_0_PIN),
#define _H1_PINS
#define _H2_PINS
#define _H3_PINS
#define _H4_PINS

#if HOTENDS > 1
  #undef _H1_PINS
  #define _H1_PINS HEATER_1_PIN, E1_AUTO_FAN_PIN, marlinAnalogInputToDigitalPin(TEMP_1_PIN),
  #if HOTENDS > 2
    #undef _H2_PINS
    #define _H2_PINS HEATER_2_PIN, E2_AUTO_FAN_PIN, marlinAnalogInputToDigitalPin(TEMP_2_PIN),
    #if HOTENDS > 3
      #undef _H3_PINS
      #define _H3_PINS HEATER_3_PIN, E3_AUTO_FAN_PIN, marlinAnalogInputToDigitalPin(TEMP_3_PIN),
      #if HOTENDS > 4
        #undef _H4_PINS
        #define _H4_PINS HEATER_4_PIN, marlinAnalogInputToDigitalPin(TEMP_4_PIN),
      #endif
    #endif
  #endif
#elif ENABLED(MIXING_EXTRUDER)
  #undef _E1_PINS
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN,
  #if MIXING_STEPPERS > 2
    #undef _E2_PINS
    #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN,
    #if MIXING_STEPPERS > 3
      #undef _E3_PINS
      #define _E3_PINS E3_STEP_PIN, E3_DIR_PIN, E3_ENABLE_PIN,
      #if MIXING_STEPPERS > 4
        #undef _E4_PINS
        #define _E4_PINS E4_STEP_PIN, E4_DIR_PIN, E4_ENABLE_PIN,
      #endif
    #endif
  #endif
#endif

#define BED_PINS HEATER_BED_PIN, marlinAnalogInputToDigitalPin(TEMP_BED_PIN),

//
// Assign endstop pins for boards with only 3 connectors
//
#ifdef X_STOP_PIN
  #if X_HOME_DIR < 0
    #define X_MIN_PIN X_STOP_PIN
    #define X_MAX_PIN -1
  #else
    #define X_MIN_PIN -1
    #define X_MAX_PIN X_STOP_PIN
  #endif
#endif

#ifdef Y_STOP_PIN
  #if Y_HOME_DIR < 0
    #define Y_MIN_PIN Y_STOP_PIN
    #define Y_MAX_PIN -1
  #else
    #define Y_MIN_PIN -1
    #define Y_MAX_PIN Y_STOP_PIN
  #endif
#endif

#ifdef Z_STOP_PIN
  #if Z_HOME_DIR < 0
    #define Z_MIN_PIN Z_STOP_PIN
    #define Z_MAX_PIN -1
  #else
    #define Z_MIN_PIN -1
    #define Z_MAX_PIN Z_STOP_PIN
  #endif
#endif

//
// Disable unused endstop / probe pins
//
#if DISABLED(Z_MIN_PROBE_ENDSTOP)
  #undef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN    -1
#endif

#if DISABLED(USE_XMAX_PLUG)
  #undef X_MAX_PIN
  #define X_MAX_PIN          -1
#endif

#if DISABLED(USE_YMAX_PLUG)
  #undef Y_MAX_PIN
  #define Y_MAX_PIN          -1
#endif

#if DISABLED(USE_ZMAX_PLUG)
  #undef Z_MAX_PIN
  #define Z_MAX_PIN          -1
#endif

#if DISABLED(USE_XMIN_PLUG)
  #undef X_MIN_PIN
  #define X_MIN_PIN          -1
#endif

#if DISABLED(USE_YMIN_PLUG)
  #undef Y_MIN_PIN
  #define Y_MIN_PIN          -1
#endif

#if DISABLED(USE_ZMIN_PLUG)
  #undef Z_MIN_PIN
  #define Z_MIN_PIN          -1
#endif

//
// Dual X-carriage, Dual Y, Dual Z support
//

#define _X2_PINS
#define _Y2_PINS
#define _Z2_PINS

#define __EPIN(p,q) E##p##_##q##_PIN
#define _EPIN(p,q) __EPIN(p,q)

// The X2 axis, if any, should be the next open extruder port
#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(X_DUAL_STEPPER_DRIVERS)
  #ifndef X2_STEP_PIN
    #define X2_STEP_PIN   _EPIN(E_STEPPERS, STEP)
    #define X2_DIR_PIN    _EPIN(E_STEPPERS, DIR)
    #define X2_ENABLE_PIN _EPIN(E_STEPPERS, ENABLE)
  #endif
  #undef _X2_PINS
  #define _X2_PINS X2_STEP_PIN, X2_DIR_PIN, X2_ENABLE_PIN,
  #define Y2_E_INDEX INCREMENT(E_STEPPERS)
#else
  #define Y2_E_INDEX E_STEPPERS
#endif

// The Y2 axis, if any, should be the next open extruder port
#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #ifndef Y2_STEP_PIN
    #define Y2_STEP_PIN   _EPIN(Y2_E_INDEX, STEP)
    #define Y2_DIR_PIN    _EPIN(Y2_E_INDEX, DIR)
    #define Y2_ENABLE_PIN _EPIN(Y2_E_INDEX, ENABLE)
  #endif
  #undef _Y2_PINS
  #define _Y2_PINS Y2_STEP_PIN, Y2_DIR_PIN, Y2_ENABLE_PIN,
  #define Z2_E_INDEX INCREMENT(Y2_E_INDEX)
#else
  #define Z2_E_INDEX Y2_E_INDEX
#endif

// The Z2 axis, if any, should be the next open extruder port
#if ENABLED(Z_DUAL_STEPPER_DRIVERS)
  #ifndef Z2_STEP_PIN
    #define Z2_STEP_PIN   _EPIN(Z2_E_INDEX, STEP)
    #define Z2_DIR_PIN    _EPIN(Z2_E_INDEX, DIR)
    #define Z2_ENABLE_PIN _EPIN(Z2_E_INDEX, ENABLE)
  #endif
  #undef _Z2_PINS
  #define _Z2_PINS Z2_STEP_PIN, Z2_DIR_PIN, Z2_ENABLE_PIN,
#endif

#define SENSITIVE_PINS { 0, 1, \
    X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, \
    Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, \
    Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, Z_MIN_PROBE_PIN, \
    PS_ON_PIN, HEATER_BED_PIN, FAN_PIN, FAN1_PIN, FAN2_PIN, CONTROLLERFAN_PIN, \
    _E0_PINS _E1_PINS _E2_PINS _E3_PINS _E4_PINS BED_PINS \
    _H0_PINS _H1_PINS _H2_PINS _H3_PINS _H4_PINS \
    _X2_PINS _Y2_PINS _Z2_PINS \
    X_MS1_PIN, X_MS2_PIN, Y_MS1_PIN, Y_MS2_PIN, Z_MS1_PIN, Z_MS2_PIN \
  }

#define HAS_DIGIPOTSS (PIN_EXISTS(DIGIPOTSS))

/**
 * Define SPI Pins: SCK, MISO, MOSI, SS
 */
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
  #define AVR_SCK_PIN  13
  #define AVR_MISO_PIN 12
  #define AVR_MOSI_PIN 11
  #define AVR_SS_PIN   10
#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284P__)
  #define AVR_SCK_PIN  7
  #define AVR_MISO_PIN 6
  #define AVR_MOSI_PIN 5
  #define AVR_SS_PIN   4
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define AVR_SCK_PIN  52
  #define AVR_MISO_PIN 50
  #define AVR_MOSI_PIN 51
  #define AVR_SS_PIN   53
#elif defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__)
  #if ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)
    // Teensy pin assignments
    #define AVR_SCK_PIN  21
    #define AVR_MISO_PIN 23
    #define AVR_MOSI_PIN 22
    #define AVR_SS_PIN   20
  #else
    // Traditional pin assignments
    #define AVR_SCK_PIN  9
    #define AVR_MISO_PIN 11
    #define AVR_MOSI_PIN 10
    #define AVR_SS_PIN   8
  #endif
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  #define AVR_SCK_PIN  10
  #define AVR_MISO_PIN 12
  #define AVR_MOSI_PIN 11
  #define AVR_SS_PIN   16
#endif

#ifndef SCK_PIN
  #define SCK_PIN  AVR_SCK_PIN
#endif
#ifndef MISO_PIN
  #define MISO_PIN AVR_MISO_PIN
#endif
#ifndef MOSI_PIN
  #define MOSI_PIN AVR_MOSI_PIN
#endif
#ifndef SS_PIN
  #define SS_PIN   AVR_SS_PIN
#endif

#endif //__PINS_H
