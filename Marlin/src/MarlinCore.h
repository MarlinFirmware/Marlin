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

#include "inc/MarlinConfig.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void stop();

// Pass true to keep steppers from timing out
void idle(bool no_stepper_sleep=false);
inline void idle_no_sleep() { idle(true); }

#if ENABLED(G38_PROBE_TARGET)
  extern uint8_t G38_move;          // Flag to tell the ISR that G38 is in progress, and the type
  extern bool G38_did_trigger;      // Flag from the ISR to indicate the endstop changed
#endif

/**
 * The axis order in all axis related arrays is X, Y, Z, E
 */
void enable_e_steppers();
void enable_all_steppers();
void disable_e_stepper(const uint8_t e);
void disable_e_steppers();
void disable_all_steppers();

void kill(PGM_P const lcd_error=nullptr, PGM_P const lcd_component=nullptr, const bool steppers_off=false);
void minkill(const bool steppers_off=false);

// Global State of the firmware
enum MarlinState : uint8_t {
  MF_INITIALIZING = 0,
  MF_STOPPED,
  MF_KILLED,
  MF_RUNNING,
  MF_SD_COMPLETE,
  MF_PAUSED,
  MF_WAITING,
};

extern MarlinState marlin_state;
inline bool IsRunning() { return marlin_state >= MF_RUNNING; }
inline bool IsStopped() { return marlin_state == MF_STOPPED; }

bool printingIsActive();
bool printJobOngoing();
bool printingIsPaused();
void startOrResumeJob();

extern bool wait_for_heatup;

#if HAS_RESUME_CONTINUE
  extern bool wait_for_user;
  void wait_for_user_response(millis_t ms=0, const bool no_sleep=false);
#endif

bool pin_is_protected(const pin_t pin);

#if HAS_SUICIDE
  inline void suicide() { OUT_WRITE(SUICIDE_PIN, SUICIDE_PIN_STATE); }
#endif

#if HAS_KILL
  #ifndef KILL_PIN_STATE
    #define KILL_PIN_STATE LOW
  #endif
  inline bool kill_state() { return READ(KILL_PIN) == KILL_PIN_STATE; }
#endif

<<<<<<< 9axis_pull
extern const char M112_KILL_STR[];
=======
#if ENABLED(G29_RETRY_AND_RECOVER)
  void event_probe_recover();
  void event_probe_failure();
#endif


extern const char NUL_STR[], M112_KILL_STR[], G28_STR[], M21_STR[], M23_STR[], M24_STR[],
                  SP_A_STR[], SP_B_STR[], SP_C_STR[], SP_E_STR[],                   
                  SP_P2_STR[], SP_T_STR[], SP_X_STR[], SP_Y_STR[], SP_Z_STR[],
                  E_LBL[], X_LBL[], Y_LBL[], Z_LBL[],                                     
                  SP_X_LBL[], SP_Y_LBL[], SP_Z_LBL[], SP_E_LBL[], 

                  I_LBL[], J_LBL[], K_LBL[], M_LBL[], O_LBL[], P_LBL[], Q_LBL[],    /**SG**/
                  SP_I_STR[], SP_J_STR[], SP_K_STR[], SP_M_STR[], SP_O_STR[], SP_P_STR[], SP_Q_STR[],  /**SG**/
                  SP_I_LBL[], SP_J_LBL[], SP_K_LBL[], SP_M_LBL[], SP_O_LBL[], SP_P_LBL[], SP_Q_LBL[];  /**SG**/
                  
>>>>>>> 87fc337 Up to 9 Axis Working. (X, Y, Z, A, B, C, D, U, E0)
