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
#pragma once

/**
 * module/servo.h
 */

#include "../inc/MarlinConfig.h"
#include "../HAL/shared/servo.h"

#if HAS_SERVO_ANGLES

  #if ENABLED(SWITCHING_EXTRUDER)
    #ifndef SWITCHING_EXTRUDER_E23_SERVO_NR
      #define SWITCHING_EXTRUDER_E23_SERVO_NR -1
    #endif
    #if EXTRUDERS > 3
      #define REQ_ANGLES 4
    #else
      #define REQ_ANGLES 2
    #endif
    #define SADATA    SWITCHING_EXTRUDER_SERVO_ANGLES
    #define ASRC(N,E) (SWITCHING_EXTRUDER_SERVO_NR == N ? asrc[E] : SWITCHING_EXTRUDER_E23_SERVO_NR == N ? asrc[E+2] : 0)
  #elif ENABLED(SWITCHING_NOZZLE)
    #define SADATA    SWITCHING_NOZZLE_SERVO_ANGLES
    #define ASRC(N,E) (SWITCHING_NOZZLE_SERVO_NR == N ? asrc[E] : 0)
  #elif defined(Z_PROBE_SERVO_NR)
    #define ASRC(N,E) (Z_PROBE_SERVO_NR == N ? asrc[E] : 0)
    #if ENABLED(BLTOUCH)
      #include "../feature/bltouch.h"
    #endif
    #ifdef BLTOUCH_ANGLES
      #define SADATA  BLTOUCH_ANGLES
    #elif defined(Z_SERVO_ANGLES)
      #define SADATA  Z_SERVO_ANGLES
    #else
      #error "Servo angles are needed!"
    #endif
  #endif

  #if ENABLED(EDITABLE_SERVO_ANGLES)
    extern uint16_t servo_angles[NUM_SERVOS][2];
    #define BASE_SERVO_ANGLES base_servo_angles
  #else
    #define BASE_SERVO_ANGLES servo_angles
  #endif

  constexpr uint16_t asrc[] = SADATA;
  #if REQ_ANGLES
    static_assert(COUNT(asrc) == REQ_ANGLES, "SWITCHING_EXTRUDER_SERVO_ANGLES needs " STRINGIFY(REQ_ANGLES) " angles.");
  #endif

  constexpr uint16_t BASE_SERVO_ANGLES [NUM_SERVOS][2] = {
      { ASRC(0,0), ASRC(0,1) }
    #if NUM_SERVOS > 1
      , { ASRC(1,0), ASRC(1,1) }
      #if NUM_SERVOS > 2
        , { ASRC(2,0), ASRC(2,1) }
        #if NUM_SERVOS > 3
          , { ASRC(3,0), ASRC(3,1) }
        #endif
      #endif
    #endif
  };

  #if HAS_Z_SERVO_PROBE
    #define DEPLOY_Z_SERVO() MOVE_SERVO(Z_PROBE_SERVO_NR, servo_angles[Z_PROBE_SERVO_NR][0])
    #define STOW_Z_SERVO() MOVE_SERVO(Z_PROBE_SERVO_NR, servo_angles[Z_PROBE_SERVO_NR][1])
  #endif

#endif // HAS_SERVO_ANGLES

#define MOVE_SERVO(I, P) servo[I].move(P)

extern HAL_SERVO_LIB servo[NUM_SERVOS];
extern void servo_init();
