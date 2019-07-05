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

#include "inc/MarlinConfig.h"

#ifdef DEBUG_GCODE_PARSER
  #include "gcode/parser.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if HAS_DRIVER(L6470)
  #include "libs/L6470/L6470_Marlin.h"
  extern uint8_t axis_known_position;
#endif

void stop();

void idle(
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    bool no_stepper_sleep = false  // pass true to keep steppers from disabling on timeout
  #endif
);

void manage_inactivity(const bool ignore_stepper_queue=false);

//
// X, Y, Z Stepper enable / disable
//
#if AXIS_DRIVER_TYPE_X(L6470)
  extern L6470 stepperX;
  #define X_enable  NOOP
  #define X_disable stepperX.free()
#elif HAS_X_ENABLE
  #define X_enable  X_ENABLE_WRITE( X_ENABLE_ON)
  #define X_disable X_ENABLE_WRITE(!X_ENABLE_ON)
#else
  #define X_enable  NOOP
  #define X_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_X2(L6470)
  extern L6470 stepperX2;
  #define X2_enable  NOOP
  #define X2_disable stepperX2.free()
#elif HAS_X2_ENABLE
  #define X2_enable  X2_ENABLE_WRITE( X_ENABLE_ON)
  #define X2_disable X2_ENABLE_WRITE(!X_ENABLE_ON)
#else
  #define X2_enable  NOOP
  #define X2_disable NOOP
#endif

#define  enable_X() do{ X_enable; X2_enable; }while(0)
#define disable_X() do{ X_disable; X2_disable; CBI(axis_known_position, X_AXIS); }while(0)

#if AXIS_DRIVER_TYPE_Y(L6470)
  extern L6470 stepperY;
  #define Y_enable  NOOP
  #define Y_disable stepperY.free()
#elif HAS_Y_ENABLE
  #define Y_enable  Y_ENABLE_WRITE( Y_ENABLE_ON)
  #define Y_disable Y_ENABLE_WRITE(!Y_ENABLE_ON)
#else
  #define Y_enable  NOOP
  #define Y_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_Y2(L6470)
  extern L6470 stepperY2;
  #define Y2_enable  NOOP
  #define Y2_disable stepperY2.free()
#elif HAS_Y2_ENABLE
  #define Y2_enable  Y2_ENABLE_WRITE( Y_ENABLE_ON)
  #define Y2_disable Y2_ENABLE_WRITE(!Y_ENABLE_ON)
#else
  #define Y2_enable  NOOP
  #define Y2_disable NOOP
#endif

#define  enable_Y() do{ Y_enable; Y2_enable; }while(0)
#define disable_Y() do{ Y_disable; Y2_disable; CBI(axis_known_position, Y_AXIS); }while(0)

#if AXIS_DRIVER_TYPE_Z(L6470)
  extern L6470 stepperZ;
  #define Z_enable  NOOP
  #define Z_disable stepperZ.free()
#elif HAS_Z_ENABLE
  #define Z_enable  Z_ENABLE_WRITE( Z_ENABLE_ON)
  #define Z_disable Z_ENABLE_WRITE(!Z_ENABLE_ON)
#else
  #define Z_enable  NOOP
  #define Z_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_Z2(L6470)
  extern L6470 stepperZ2;
  #define Z2_enable  NOOP
  #define Z2_disable stepperZ2.free()
#elif HAS_Z2_ENABLE
  #define Z2_enable  Z2_ENABLE_WRITE( Z_ENABLE_ON)
  #define Z2_disable Z2_ENABLE_WRITE(!Z_ENABLE_ON)
#else
  #define Z2_enable  NOOP
  #define Z2_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_Z3(L6470)
  extern L6470 stepperZ3;
  #define Z3_enable  NOOP
  #define Z3_disable stepperZ3.free()
#elif HAS_Z3_ENABLE
  #define Z3_enable  Z3_ENABLE_WRITE( Z_ENABLE_ON)
  #define Z3_disable Z3_ENABLE_WRITE(!Z_ENABLE_ON)
#else
  #define Z3_enable  NOOP
  #define Z3_disable NOOP
#endif

#define  enable_Z() do{ Z_enable; Z2_enable; Z3_enable; }while(0)
#define disable_Z() do{ Z_disable; Z2_disable; Z3_disable; CBI(axis_known_position, Z_AXIS); }while(0)

//
// Extruder Stepper enable / disable
//

// define the individual enables/disables
#if AXIS_DRIVER_TYPE_E0(L6470)
  extern L6470 stepperE0;
  #define  E0_enable NOOP
  #define E0_disable do{ stepperE0.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif HAS_E0_ENABLE
  #define  E0_enable E0_ENABLE_WRITE( E_ENABLE_ON)
  #define E0_disable E0_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E0_enable NOOP
  #define E0_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_E1(L6470)
  extern L6470 stepperE1;
  #define  E1_enable NOOP
  #define E1_disable do{ stepperE1.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 1 && HAS_E1_ENABLE
  #define  E1_enable E1_ENABLE_WRITE( E_ENABLE_ON)
  #define E1_disable E1_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E1_enable NOOP
  #define E1_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_E2(L6470)
  extern L6470 stepperE2;
  #define  E2_enable NOOP
  #define E2_disable do{ stepperE2.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 2 && HAS_E2_ENABLE
  #define  E2_enable E2_ENABLE_WRITE( E_ENABLE_ON)
  #define E2_disable E2_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E2_enable NOOP
  #define E2_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_E3(L6470)
  extern L6470 stepperE3;
  #define  E3_enable NOOP
  #define E3_disable do{ stepperE3.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 3 && HAS_E3_ENABLE
  #define  E3_enable E3_ENABLE_WRITE( E_ENABLE_ON)
  #define E3_disable E3_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E3_enable NOOP
  #define E3_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_E4(L6470)
  extern L6470 stepperE4;
  #define  E4_enable NOOP
  #define E4_disable do{ stepperE4.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 4 && HAS_E4_ENABLE
  #define  E4_enable E4_ENABLE_WRITE( E_ENABLE_ON)
  #define E4_disable E4_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E4_enable NOOP
  #define E4_disable NOOP
#endif

#if AXIS_DRIVER_TYPE_E5(L6470)
  extern L6470 stepperE5;
  #define  E5_enable NOOP
  #define E5_disable do{ stepperE5.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 5 && HAS_E5_ENABLE
  #define  E5_enable E5_ENABLE_WRITE( E_ENABLE_ON)
  #define E5_disable E5_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E5_enable NOOP
  #define E5_disable NOOP
#endif

#if ENABLED(MIXING_EXTRUDER)

  /**
   * Mixing steppers synchronize their enable (and direction) together
   */
  #if MIXING_STEPPERS > 5
    #define  enable_E0() { E0_enable;  E1_enable;  E2_enable;  E3_enable;  E4_enable;  E5_enable; }
    #define disable_E0() { E0_disable; E1_disable; E2_disable; E3_disable; E4_disable; E5_disable; }
  #elif MIXING_STEPPERS > 4
    #define  enable_E0() { E0_enable;  E1_enable;  E2_enable;  E3_enable;  E4_enable; }
    #define disable_E0() { E0_disable; E1_disable; E2_disable; E3_disable; E4_disable; }
  #elif MIXING_STEPPERS > 3
    #define  enable_E0() { E0_enable;  E1_enable;  E2_enable;  E3_enable; }
    #define disable_E0() { E0_disable; E1_disable; E2_disable; E3_disable; }
  #elif MIXING_STEPPERS > 2
    #define  enable_E0() { E0_enable;  E1_enable;  E2_enable; }
    #define disable_E0() { E0_disable; E1_disable; E2_disable; }
  #else
    #define  enable_E0() { E0_enable;  E1_enable; }
    #define disable_E0() { E0_disable; E1_disable; }
  #endif
  #define  enable_E1() NOOP
  #define disable_E1() NOOP
  #define  enable_E2() NOOP
  #define disable_E2() NOOP
  #define  enable_E3() NOOP
  #define disable_E3() NOOP
  #define  enable_E4() NOOP
  #define disable_E4() NOOP
  #define  enable_E5() NOOP
  #define disable_E5() NOOP

#else // !MIXING_EXTRUDER

  #if HAS_E0_ENABLE
    #define  enable_E0() E0_enable
    #define disable_E0() E0_disable
  #else
    #define  enable_E0() NOOP
    #define disable_E0() NOOP
  #endif

  #if E_STEPPERS > 1 && HAS_E1_ENABLE
    #define  enable_E1() E1_enable
    #define disable_E1() E1_disable
  #else
    #define  enable_E1() NOOP
    #define disable_E1() NOOP
  #endif

  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define  enable_E2() E2_enable
    #define disable_E2() E2_disable
  #else
    #define  enable_E2() NOOP
    #define disable_E2() NOOP
  #endif

  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define  enable_E3() E3_enable
    #define disable_E3() E3_disable
  #else
    #define  enable_E3() NOOP
    #define disable_E3() NOOP
  #endif

  #if E_STEPPERS > 4 && HAS_E4_ENABLE
    #define  enable_E4() E4_enable
    #define disable_E4() E4_disable
  #else
    #define  enable_E4() NOOP
    #define disable_E4() NOOP
  #endif

  #if E_STEPPERS > 5 && HAS_E5_ENABLE
    #define  enable_E5() E5_enable
    #define disable_E5() E5_disable
  #else
    #define  enable_E5() NOOP
    #define disable_E5() NOOP
  #endif

#endif // !MIXING_EXTRUDER

#if ENABLED(EXPERIMENTAL_I2CBUS)
  #include "feature/twibus.h"
  extern TWIBus i2c;
#endif

#if ENABLED(G38_PROBE_TARGET)
  extern uint8_t G38_move;          // Flag to tell the ISR that G38 is in progress, and the type
  extern bool G38_did_trigger;      // Flag from the ISR to indicate the endstop changed
#endif

/**
 * The axis order in all axis related arrays is X, Y, Z, E
 */
void enable_all_steppers();
void disable_e_stepper(const uint8_t e);
void disable_e_steppers();
void disable_all_steppers();

void kill(PGM_P const lcd_msg=nullptr);
void minkill();

void quickstop_stepper();

extern bool Running;
inline bool IsRunning() { return  Running; }
inline bool IsStopped() { return !Running; }

extern bool wait_for_heatup;

#if HAS_RESUME_CONTINUE
  extern bool wait_for_user;
#endif

#if HAS_AUTO_REPORTING || ENABLED(HOST_KEEPALIVE_FEATURE)
  extern bool suspend_auto_report;
#endif

// Inactivity shutdown timer
extern millis_t max_inactive_time, stepper_inactive_time;

#if ENABLED(USE_CONTROLLER_FAN)
  extern uint8_t controllerfan_speed;
#endif

#if HAS_POWER_SWITCH
  extern bool powersupply_on;
  #define PSU_PIN_ON()  do{ OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE); powersupply_on = true; }while(0)
  #define PSU_PIN_OFF() do{ OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP); powersupply_on = false; }while(0)
  #if ENABLED(AUTO_POWER_CONTROL)
    #define PSU_ON()  powerManager.power_on()
    #define PSU_OFF() powerManager.power_off()
  #else
    #define PSU_ON()  PSU_PIN_ON()
    #define PSU_OFF() PSU_PIN_OFF()
  #endif
#endif

bool pin_is_protected(const pin_t pin);
void protected_pin_err();

#if HAS_SUICIDE
  inline void suicide() { OUT_WRITE(SUICIDE_PIN, LOW); }
#endif

#if HAS_FILAMENT_SENSOR
  void event_filament_runout();
#endif

#if ENABLED(G29_RETRY_AND_RECOVER)
  void event_probe_recover();
  void event_probe_failure();
#endif
