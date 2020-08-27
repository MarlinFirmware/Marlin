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

//
// Prepare a list of protected pins for M42/M43
//

#if PIN_EXISTS(X_MIN)
  #define _X_MIN X_MIN_PIN,
#else
  #define _X_MIN
#endif
#if PIN_EXISTS(X_MAX)
  #define _X_MAX X_MAX_PIN,
#else
  #define _X_MAX
#endif
#if PIN_EXISTS(X_CS) && AXIS_HAS_SPI(X)
  #define _X_CS X_CS_PIN,
#else
  #define _X_CS
#endif
#if PIN_EXISTS(X_MS1)
  #define _X_MS1 X_MS1_PIN,
#else
  #define _X_MS1
#endif
#if PIN_EXISTS(X_MS2)
  #define _X_MS2 X_MS2_PIN,
#else
  #define _X_MS2
#endif
#if PIN_EXISTS(X_MS3)
  #define _X_MS3 X_MS3_PIN,
#else
  #define _X_MS3
#endif

#define _X_PINS X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, _X_MIN _X_MAX _X_MS1 _X_MS2 _X_MS3 _X_CS

#if PIN_EXISTS(Y_MIN)
  #define _Y_MIN Y_MIN_PIN,
#else
  #define _Y_MIN
#endif
#if PIN_EXISTS(Y_MAX)
  #define _Y_MAX Y_MAX_PIN,
#else
  #define _Y_MAX
#endif
#if PIN_EXISTS(Y_CS) && AXIS_HAS_SPI(Y)
  #define _Y_CS Y_CS_PIN,
#else
  #define _Y_CS
#endif
#if PIN_EXISTS(Y_MS1)
  #define _Y_MS1 Y_MS1_PIN,
#else
  #define _Y_MS1
#endif
#if PIN_EXISTS(Y_MS2)
  #define _Y_MS2 Y_MS2_PIN,
#else
  #define _Y_MS2
#endif
#if PIN_EXISTS(Y_MS3)
  #define _Y_MS3 Y_MS3_PIN,
#else
  #define _Y_MS3
#endif

#define _Y_PINS Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, _Y_MIN _Y_MAX _Y_MS1 _Y_MS2 _Y_MS3 _Y_CS

#if PIN_EXISTS(Z_MIN)
  #define _Z_MIN Z_MIN_PIN,
#else
  #define _Z_MIN
#endif
#if PIN_EXISTS(Z_MAX)
  #define _Z_MAX Z_MAX_PIN,
#else
  #define _Z_MAX
#endif
#if PIN_EXISTS(Z_CS) && AXIS_HAS_SPI(Z)
  #define _Z_CS Z_CS_PIN,
#else
  #define _Z_CS
#endif
#if PIN_EXISTS(Z_MS1)
  #define _Z_MS1 Z_MS1_PIN,
#else
  #define _Z_MS1
#endif
#if PIN_EXISTS(Z_MS2)
  #define _Z_MS2 Z_MS2_PIN,
#else
  #define _Z_MS2
#endif
#if PIN_EXISTS(Z_MS3)
  #define _Z_MS3 Z_MS3_PIN,
#else
  #define _Z_MS3
#endif

#define _Z_PINS Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, _Z_MIN _Z_MAX _Z_MS1 _Z_MS2 _Z_MS3 _Z_CS

//
// Extruder Chip Select, Digital Micro-steps
//

// Mixing stepper, Switching stepper, or regular stepper
#define E_NEEDED(N) (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > N) \
                 || (ENABLED(SWITCHING_EXTRUDER) && E_STEPPERS > N) \
                 || (NONE(SWITCHING_EXTRUDER, MIXING_EXTRUDER) && EXTRUDERS > N)

#define _E0_CS
#define _E0_MS1
#define _E0_MS2
#define _E0_MS3

#if E_NEEDED(0)
  #if PIN_EXISTS(E0_CS) && AXIS_HAS_SPI(E0)
    #undef _E0_CS
    #define _E0_CS E0_CS_PIN,
  #endif
  #if PIN_EXISTS(E0_MS1)
    #undef _E0_MS1
    #define _E0_MS1 E0_MS1_PIN,
  #endif
  #if PIN_EXISTS(E0_MS2)
    #undef _E0_MS2
    #define _E0_MS2 E0_MS2_PIN,
  #endif
  #if PIN_EXISTS(E0_MS3)
    #undef _E0_MS3
    #define _E0_MS3 E0_MS3_PIN,
  #endif
#endif

#define _E1_CS
#define _E1_MS1
#define _E1_MS2
#define _E1_MS3

#if E_NEEDED(1)
  #if PIN_EXISTS(E1_CS) && AXIS_HAS_SPI(E1)
    #undef _E1_CS
    #define _E1_CS E1_CS_PIN,
  #endif
  #if PIN_EXISTS(E1_MS1)
    #undef _E1_MS1
    #define _E1_MS1 E1_MS1_PIN,
  #endif
  #if PIN_EXISTS(E1_MS2)
    #undef _E1_MS2
    #define _E1_MS2 E1_MS2_PIN,
  #endif
  #if PIN_EXISTS(E1_MS3)
    #undef _E1_MS3
    #define _E1_MS3 E1_MS3_PIN,
  #endif
#endif

#define _E2_CS
#define _E2_MS1
#define _E2_MS2
#define _E2_MS3

#if E_NEEDED(2)
  #if PIN_EXISTS(E2_CS) && AXIS_HAS_SPI(E2)
    #undef _E2_CS
    #define _E2_CS E2_CS_PIN,
  #endif
  #if PIN_EXISTS(E2_MS1)
    #undef _E2_MS1
    #define _E2_MS1 E2_MS1_PIN,
  #endif
  #if PIN_EXISTS(E2_MS2)
    #undef _E2_MS2
    #define _E2_MS2 E2_MS2_PIN,
  #endif
  #if PIN_EXISTS(E2_MS3)
    #undef _E2_MS3
    #define _E2_MS3 E2_MS3_PIN,
  #endif
#endif

#define _E3_CS
#define _E3_MS1
#define _E3_MS2
#define _E3_MS3

#if E_NEEDED(3)
  #if PIN_EXISTS(E3_CS) && AXIS_HAS_SPI(E3)
    #undef _E3_CS
    #define _E3_CS E3_CS_PIN,
  #endif
  #if PIN_EXISTS(E3_MS1)
    #undef _E3_MS1
    #define _E3_MS1 E3_MS1_PIN,
  #endif
  #if PIN_EXISTS(E3_MS2)
    #undef _E3_MS2
    #define _E3_MS2 E3_MS2_PIN,
  #endif
  #if PIN_EXISTS(E3_MS3)
    #undef _E3_MS3
    #define _E3_MS3 E3_MS3_PIN,
  #endif
#endif

#define _E4_CS
#define _E4_MS1
#define _E4_MS2
#define _E4_MS3

#if E_NEEDED(4)
  #if PIN_EXISTS(E4_CS) && AXIS_HAS_SPI(E4)
    #undef _E4_CS
    #define _E4_CS E4_CS_PIN,
  #endif
  #if PIN_EXISTS(E4_MS1)
    #undef _E4_MS1
    #define _E4_MS1 E4_MS1_PIN,
  #endif
  #if PIN_EXISTS(E4_MS2)
    #undef _E4_MS2
    #define _E4_MS2 E4_MS2_PIN,
  #endif
  #if PIN_EXISTS(E4_MS3)
    #undef _E4_MS3
    #define _E4_MS3 E4_MS3_PIN,
  #endif
#endif

#define _E5_CS
#define _E5_MS1
#define _E5_MS2
#define _E5_MS3

#if E_NEEDED(5)
  #if PIN_EXISTS(E5_CS) && AXIS_HAS_SPI(E5)
    #undef _E5_CS
    #define _E5_CS E5_CS_PIN,
  #endif
  #if PIN_EXISTS(E5_MS1)
    #undef _E5_MS1
    #define _E5_MS1 E5_MS1_PIN,
  #endif
  #if PIN_EXISTS(E5_MS2)
    #undef _E5_MS2
    #define _E5_MS2 E5_MS2_PIN,
  #endif
  #if PIN_EXISTS(E5_MS3)
    #undef _E5_MS3
    #define _E5_MS3 E5_MS3_PIN,
  #endif
#endif

#define _E6_CS
#define _E6_MS1
#define _E6_MS2
#define _E6_MS3

#if E_NEEDED(6)
  #if PIN_EXISTS(E6_CS) && AXIS_HAS_SPI(E6)
    #undef _E6_CS
    #define _E6_CS E6_CS_PIN,
  #endif
  #if PIN_EXISTS(E6_MS2)
    #undef _E6_MS2
    #define _E6_MS2 E6_MS2_PIN,
  #endif
  #if PIN_EXISTS(E6_MS3)
    #undef _E6_MS3
    #define _E6_MS3 E6_MS3_PIN,
  #endif
  #if PIN_EXISTS(E6_MS4)
    #undef _E6_MS4
    #define _E6_MS4 E6_MS4_PIN,
  #endif
#endif

#define _E7_CS
#define _E7_MS1
#define _E7_MS2
#define _E7_MS3

#if E_NEEDED(7)
  #if PIN_EXISTS(E7_CS) && AXIS_HAS_SPI(E7)
    #undef _E7_CS
    #define _E7_CS E7_CS_PIN,
  #endif
  #if PIN_EXISTS(E7_MS3)
    #undef _E7_MS3
    #define _E7_MS3 E7_MS3_PIN,
  #endif
  #if PIN_EXISTS(E7_MS4)
    #undef _E7_MS4
    #define _E7_MS4 E7_MS4_PIN,
  #endif
  #if PIN_EXISTS(E7_MS5)
    #undef _E7_MS5
    #define _E7_MS5 E7_MS5_PIN,
  #endif
#endif

//
// E Steppers
//

#define _E0_PINS
#define _E1_PINS
#define _E2_PINS
#define _E3_PINS
#define _E4_PINS
#define _E5_PINS
#define _E6_PINS
#define _E7_PINS

#if EXTRUDERS
  #undef _E0_PINS
  #define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, _E0_CS _E0_MS1 _E0_MS2 _E0_MS3
#endif

#if ENABLED(SWITCHING_EXTRUDER)
                      // Tools 0 and 1 use E0
  #if EXTRUDERS > 2   // Tools 2 and 3 use E1
    #undef _E1_PINS
    #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, _E1_CS _E1_MS1 _E1_MS2 _E1_MS3
    #if EXTRUDERS > 4 // Tools 4 and 5 use E2
      #undef _E2_PINS
      #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, _E2_CS _E2_MS1 _E2_MS2 _E2_MS3
    #endif
  #endif

#elif EXTRUDERS > 1 || ENABLED(MIXING_EXTRUDER)

  #undef _E1_PINS
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, _E1_CS _E1_MS1 _E1_MS2 _E1_MS3
  #if EXTRUDERS > 2 || (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > 2)
    #undef _E2_PINS
    #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, _E2_CS _E2_MS1 _E2_MS2 _E2_MS3
    #if EXTRUDERS > 3 || (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > 3)
      #undef _E3_PINS
      #define _E3_PINS E3_STEP_PIN, E3_DIR_PIN, E3_ENABLE_PIN, _E3_CS _E3_MS1 _E3_MS2 _E3_MS3
      #if EXTRUDERS > 4 || (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > 4)
        #undef _E4_PINS
        #define _E4_PINS E4_STEP_PIN, E4_DIR_PIN, E4_ENABLE_PIN, _E4_CS _E4_MS1 _E4_MS2 _E4_MS3
        #if EXTRUDERS > 5 || (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > 5)
          #undef _E5_PINS
          #define _E5_PINS E5_STEP_PIN, E5_DIR_PIN, E5_ENABLE_PIN, _E5_CS _E5_MS1 _E5_MS2 _E5_MS3
          #if EXTRUDERS > 6 || (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > 6)
            #undef _E6_PINS
            #define _E6_PINS E6_STEP_PIN, E6_DIR_PIN, E6_ENABLE_PIN, _E6_CS _E6_MS1 _E6_MS2 _E6_MS3
            #if EXTRUDERS > 7 || (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > 7)
              #undef _E7_PINS
              #define _E7_PINS E7_STEP_PIN, E7_DIR_PIN, E7_ENABLE_PIN, _E7_CS _E7_MS1 _E7_MS2 _E7_MS3
            #endif // EXTRUDERS > 7 || MIXING_EXTRUDER > 7
          #endif // EXTRUDERS > 6 || MIXING_EXTRUDER > 6
        #endif // EXTRUDERS > 5 || MIXING_EXTRUDER > 5
      #endif // EXTRUDERS > 4 || MIXING_EXTRUDER > 4
    #endif // EXTRUDERS > 3 || MIXING_EXTRUDER > 3
  #endif // EXTRUDERS > 2 || MIXING_EXTRUDER > 2

#endif // EXTRUDERS > 1 || MIXING_EXTRUDER

//
// Heaters, Fans, Temp Sensors
//

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN -1
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN -1
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN -1
#endif
#ifndef E3_AUTO_FAN_PIN
  #define E3_AUTO_FAN_PIN -1
#endif
#ifndef E4_AUTO_FAN_PIN
  #define E4_AUTO_FAN_PIN -1
#endif
#ifndef E5_AUTO_FAN_PIN
  #define E5_AUTO_FAN_PIN -1
#endif
#ifndef E6_AUTO_FAN_PIN
  #define E6_AUTO_FAN_PIN -1
#endif
#ifndef E7_AUTO_FAN_PIN
  #define E7_AUTO_FAN_PIN -1
#endif

#define _H0_PINS
#define _H1_PINS
#define _H2_PINS
#define _H3_PINS
#define _H4_PINS
#define _H5_PINS
#define _H6_PINS
#define _H7_PINS

#if HAS_HOTEND
  #undef _H0_PINS
  #define _H0_PINS HEATER_0_PIN, E0_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_0_PIN),
  #if HAS_MULTI_HOTEND
    #undef _H1_PINS
    #define _H1_PINS HEATER_1_PIN, E1_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_1_PIN),
    #if HOTENDS > 2
      #undef _H2_PINS
      #define _H2_PINS HEATER_2_PIN, E2_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_2_PIN),
      #if HOTENDS > 3
        #undef _H3_PINS
        #define _H3_PINS HEATER_3_PIN, E3_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_3_PIN),
        #if HOTENDS > 4
          #undef _H4_PINS
          #define _H4_PINS HEATER_4_PIN, E4_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_4_PIN),
          #if HOTENDS > 5
            #undef _H5_PINS
            #define _H5_PINS HEATER_5_PIN, E5_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_5_PIN),
            #if HOTENDS > 6
              #undef _H6_PINS
              #define _H6_PINS HEATER_6_PIN, E6_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_6_PIN),
              #if HOTENDS > 7
                #undef _H7_PINS
                #define _H7_PINS HEATER_7_PIN, E7_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_7_PIN),
              #endif // HOTENDS > 7
            #endif // HOTENDS > 6
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HAS_MULTI_HOTEND
#endif // HOTENDS

//
// Dual X, Dual Y, Multi-Z
// Chip Select and Digital Micro-stepping
//

#if EITHER(DUAL_X_CARRIAGE, X_DUAL_STEPPER_DRIVERS)
  #if PIN_EXISTS(X2_CS) && AXIS_HAS_SPI(X2)
    #define _X2_CS X2_CS_PIN,
  #else
    #define _X2_CS
  #endif
  #if PIN_EXISTS(X2_MS1)
    #define _X2_MS1 X2_MS1_PIN,
  #else
    #define _X2_MS1
  #endif
  #if PIN_EXISTS(X2_MS2)
    #define _X2_MS2 X2_MS2_PIN,
  #else
    #define _X2_MS2
  #endif
  #if PIN_EXISTS(X2_MS3)
    #define _X2_MS3 X2_MS3_PIN,
  #else
    #define _X2_MS3
  #endif
  #define _X2_PINS X2_STEP_PIN, X2_DIR_PIN, X2_ENABLE_PIN, _X2_CS _X2_MS1 _X2_MS2 _X2_MS3
#else
  #define _X2_PINS
#endif

#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #if PIN_EXISTS(Y2_CS) && AXIS_HAS_SPI(Y2)
    #define _Y2_CS Y2_CS_PIN,
  #else
    #define _Y2_CS
  #endif
  #if PIN_EXISTS(Y2_MS1)
    #define _Y2_MS1 Y2_MS1_PIN,
  #else
    #define _Y2_MS1
  #endif
  #if PIN_EXISTS(Y2_MS2)
    #define _Y2_MS2 Y2_MS2_PIN,
  #else
    #define _Y2_MS2
  #endif
  #if PIN_EXISTS(Y2_MS3)
    #define _Y2_MS3 Y2_MS3_PIN,
  #else
    #define _Y2_MS3
  #endif
  #define _Y2_PINS Y2_STEP_PIN, Y2_DIR_PIN, Y2_ENABLE_PIN, _Y2_CS _Y2_MS1 _Y2_MS2 _Y2_MS3
#else
  #define _Y2_PINS
#endif

#if NUM_Z_STEPPER_DRIVERS >= 2
  #if PIN_EXISTS(Z2_CS) && AXIS_HAS_SPI(Z2)
    #define _Z2_CS Z2_CS_PIN,
  #else
    #define _Z2_CS
  #endif
  #if PIN_EXISTS(Z2_MS1)
    #define _Z2_MS1 Z2_MS1_PIN,
  #else
    #define _Z2_MS1
  #endif
  #if PIN_EXISTS(Z2_MS2)
    #define _Z2_MS2 Z2_MS2_PIN,
  #else
    #define _Z2_MS2
  #endif
  #if PIN_EXISTS(Z2_MS3)
    #define _Z2_MS3 Z2_MS3_PIN,
  #else
    #define _Z2_MS3
  #endif
  #define _Z2_PINS Z2_STEP_PIN, Z2_DIR_PIN, Z2_ENABLE_PIN, _Z2_CS _Z2_MS1 _Z2_MS2 _Z2_MS3
#else
  #define _Z2_PINS
#endif

#if NUM_Z_STEPPER_DRIVERS >= 3
  #if PIN_EXISTS(Z3_CS) && AXIS_HAS_SPI(Z3)
    #define _Z3_CS Z3_CS_PIN,
  #else
    #define _Z3_CS
  #endif
  #if PIN_EXISTS(Z3_MS1)
    #define _Z3_MS1 Z3_MS1_PIN,
  #else
    #define _Z3_MS1
  #endif
  #if PIN_EXISTS(Z3_MS2)
    #define _Z3_MS2 Z3_MS2_PIN,
  #else
    #define _Z3_MS2
  #endif
  #if PIN_EXISTS(Z3_MS3)
    #define _Z3_MS3 Z3_MS3_PIN,
  #else
    #define _Z3_MS3
  #endif
  #define _Z3_PINS Z3_STEP_PIN, Z3_DIR_PIN, Z3_ENABLE_PIN, _Z3_CS _Z3_MS1 _Z3_MS2 _Z3_MS3
#else
  #define _Z3_PINS
#endif

#if NUM_Z_STEPPER_DRIVERS >= 4
  #if PIN_EXISTS(Z4_CS) && AXIS_HAS_SPI(Z4)
    #define _Z4_CS Z4_CS_PIN,
  #else
    #define _Z4_CS
  #endif
  #if PIN_EXISTS(Z4_MS1)
    #define _Z4_MS1 Z4_MS1_PIN,
  #else
    #define _Z4_MS1
  #endif
  #if PIN_EXISTS(Z4_MS2)
    #define _Z4_MS2 Z4_MS2_PIN,
  #else
    #define _Z4_MS2
  #endif
  #if PIN_EXISTS(Z4_MS3)
    #define _Z4_MS3 Z4_MS3_PIN,
  #else
    #define _Z4_MS3
  #endif
  #define _Z4_PINS Z4_STEP_PIN, Z4_DIR_PIN, Z4_ENABLE_PIN, _Z4_CS _Z4_MS1 _Z4_MS2 _Z4_MS3
#else
  #define _Z4_PINS
#endif

//
// Generate the final Sensitive Pins array,
// keeping the array as small as possible.
//

#if PIN_EXISTS(PS_ON)
  #define _PS_ON PS_ON_PIN,
#else
  #define _PS_ON
#endif

#if HAS_BED_PROBE && PIN_EXISTS(Z_MIN_PROBE)
  #define _Z_PROBE Z_MIN_PROBE_PIN,
#else
  #define _Z_PROBE
#endif

#if TEMP_SENSOR_BED && PIN_EXISTS(HEATER_BED)
  #define _HEATER_BED HEATER_BED_PIN,
#else
  #define _HEATER_BED
#endif

#if PIN_EXISTS(FAN)
  #define _FAN0 FAN_PIN,
#else
  #define _FAN0
#endif
#if PIN_EXISTS(FAN1)
  #define _FAN1 FAN1_PIN,
#else
  #define _FAN1
#endif
#if PIN_EXISTS(FAN2)
  #define _FAN2 FAN2_PIN,
#else
  #define _FAN2
#endif
#if PIN_EXISTS(FAN3)
  #define _FAN3 FAN3_PIN,
#else
  #define _FAN3
#endif
#if PIN_EXISTS(FAN4)
  #define _FAN4 FAN4_PIN,
#else
  #define _FAN4
#endif
#if PIN_EXISTS(FAN5)
  #define _FAN5 FAN5_PIN,
#else
  #define _FAN5
#endif
#if PIN_EXISTS(FAN6)
  #define _FAN6 FAN6_PIN,
#else
  #define _FAN6
#endif
#if PIN_EXISTS(FAN7)
  #define _FAN7 FAN7_PIN,
#else
  #define _FAN7
#endif
#if PIN_EXISTS(CONTROLLER_FAN)
  #define _FANC CONTROLLER_FAN_PIN,
#else
  #define _FANC
#endif

#if PIN_EXISTS(HEATER_BED) && PIN_EXISTS(TEMP_BED)
  #define _BED_PINS HEATER_BED_PIN, analogInputToDigitalPin(TEMP_BED_PIN),
#else
  #define _BED_PINS
#endif

#if PIN_EXISTS(TEMP_CHAMBER)
  #define __CHAMBER_PINS CHAMBER_AUTO_FAN_PIN, analogInputToDigitalPin(TEMP_CHAMBER_PIN),
#else
  #define __CHAMBER_PINS
#endif
#if PIN_EXISTS(HEATER_CHAMBER)
  #define _CHAMBER_PINS __CHAMBER_PINS HEATER_CHAMBER_PIN,
#else
  #define _CHAMBER_PINS
#endif

#ifndef HAL_SENSITIVE_PINS
  #define HAL_SENSITIVE_PINS
#endif

#define SENSITIVE_PINS { \
  _X_PINS _Y_PINS _Z_PINS _X2_PINS _Y2_PINS _Z2_PINS _Z3_PINS _Z4_PINS _Z_PROBE \
  _E0_PINS _E1_PINS _E2_PINS _E3_PINS _E4_PINS _E5_PINS _E6_PINS _E7_PINS \
  _H0_PINS _H1_PINS _H2_PINS _H3_PINS _H4_PINS _H5_PINS _H6_PINS _H7_PINS \
  _PS_ON _HEATER_BED _FAN0 _FAN1 _FAN2 _FAN3 _FAN4 _FAN5 _FAN6 _FAN7 _FANC \
  _BED_PINS _CHAMBER_PINS HAL_SENSITIVE_PINS \
}
