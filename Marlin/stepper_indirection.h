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

/**
  stepper_indirection.h - stepper motor driver indirection macros
    allow some stepper functions to be done via SPI/I2c instead of
    direct pin manipulation, or to run Marlin in simulation mode.

  Part of Marlin

  Copyright (c) 2015 Dominik Wenger

  Marlin is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Marlin is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Marlin.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STEPPER_INDIRECTION_H
#define STEPPER_INDIRECTION_H

#include "MarlinConfig.h"

#if ENABLED(MARLIN_SIMULATION_BUILD)
  #include "stepper_indirection_sim.h"
#else
  #include "stepper_indirection_real.h"
#endif

/**
 * Extruder indirection for the single E axis
 */
#if ENABLED(SWITCHING_EXTRUDER)
  #define E_STEP_WRITE(v) E0_STEP_WRITE(v)
  #define NORM_E_DIR() E0_DIR_WRITE(current_block->active_extruder ?  INVERT_E0_DIR : !INVERT_E0_DIR)
  #define  REV_E_DIR() E0_DIR_WRITE(current_block->active_extruder ? !INVERT_E0_DIR :  INVERT_E0_DIR)
#elif EXTRUDERS > 3
  #define E_STEP_WRITE(v) { switch (current_block->active_extruder) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); break; case 3: E3_STEP_WRITE(v); } }
  #define NORM_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); } }
  #define REV_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(INVERT_E3_DIR); } }
#elif EXTRUDERS > 2
  #define E_STEP_WRITE(v) { switch (current_block->active_extruder) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); } }
  #define NORM_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); } }
  #define REV_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(INVERT_E2_DIR); } }
#elif EXTRUDERS > 1
  #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
    #define E_STEP_WRITE(v) { if (extruder_duplication_enabled) { E0_STEP_WRITE(v); E1_STEP_WRITE(v); } else if (current_block->active_extruder == 0) { E0_STEP_WRITE(v); } else { E1_STEP_WRITE(v); } }
    #define NORM_E_DIR() { if (extruder_duplication_enabled) { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); } else if (current_block->active_extruder == 0) { E0_DIR_WRITE(!INVERT_E0_DIR); } else { E1_DIR_WRITE(!INVERT_E1_DIR); } }
    #define REV_E_DIR() { if (extruder_duplication_enabled) { E0_DIR_WRITE(INVERT_E0_DIR); E1_DIR_WRITE(INVERT_E1_DIR); } else if (current_block->active_extruder == 0) { E0_DIR_WRITE(INVERT_E0_DIR); } else { E1_DIR_WRITE(INVERT_E1_DIR); } }
  #else
    #define E_STEP_WRITE(v) { if (current_block->active_extruder == 0) { E0_STEP_WRITE(v); } else { E1_STEP_WRITE(v); } }
    #define NORM_E_DIR() { if (current_block->active_extruder == 0) { E0_DIR_WRITE(!INVERT_E0_DIR); } else { E1_DIR_WRITE(!INVERT_E1_DIR); } }
    #define REV_E_DIR() { if (current_block->active_extruder == 0) { E0_DIR_WRITE(INVERT_E0_DIR); } else { E1_DIR_WRITE(INVERT_E1_DIR); } }
  #endif
#elif ENABLED(MIXING_EXTRUDER)
  #define E_STEP_WRITE(v) NOOP /* not used for mixing extruders! */
  #if MIXING_STEPPERS > 3
    #define En_STEP_WRITE(n,v) { switch (n) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); break; case 3: E3_STEP_WRITE(v); } }
    #define NORM_E_DIR() { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); E2_DIR_WRITE(!INVERT_E2_DIR); E3_DIR_WRITE(!INVERT_E3_DIR); }
    #define REV_E_DIR()  { E0_DIR_WRITE( INVERT_E0_DIR); E1_DIR_WRITE( INVERT_E1_DIR); E2_DIR_WRITE( INVERT_E2_DIR); E3_DIR_WRITE( INVERT_E3_DIR); }
  #elif MIXING_STEPPERS > 2
    #define En_STEP_WRITE(n,v) { switch (n) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); } }
    #define NORM_E_DIR() { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); E2_DIR_WRITE(!INVERT_E2_DIR); }
    #define REV_E_DIR()  { E0_DIR_WRITE( INVERT_E0_DIR); E1_DIR_WRITE( INVERT_E1_DIR); E2_DIR_WRITE( INVERT_E2_DIR); }
  #else
    #define En_STEP_WRITE(n,v) { switch (n) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); } }
    #define NORM_E_DIR() { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); }
    #define REV_E_DIR()  { E0_DIR_WRITE( INVERT_E0_DIR); E1_DIR_WRITE( INVERT_E1_DIR); }
  #endif
#else
  #define E_STEP_WRITE(v) E0_STEP_WRITE(v)
  #define NORM_E_DIR() E0_DIR_WRITE(!INVERT_E0_DIR)
  #define REV_E_DIR() E0_DIR_WRITE(INVERT_E0_DIR)
#endif

#endif // STEPPER_INDIRECTION_H
