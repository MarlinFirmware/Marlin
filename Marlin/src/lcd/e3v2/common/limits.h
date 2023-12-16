/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/*****************************************************************************
 * @file     lcd/e3v2/common/limits.h
 * @brief    Limits for UI values
 ****************************************************************************/

#include "../../../inc/MarlinConfig.h"

// If max edit values are not specified use default * DEFAULT_MAX_MULTIPLIER
#define DEFAULT_MAX_MULTIPLIER 2

//
// Feedrate limits
//
#define MIN_FEEDRATE_EDIT_VALUE 1

constexpr xyze_float_t min_feedrate_edit_values = LOGICAL_AXIS_ARRAY_1(MIN_FEEDRATE_EDIT_VALUE),
                       default_max_feedrate = DEFAULT_MAX_FEEDRATE,
                       max_feedrate_edit_values =
                         #ifdef MAX_FEEDRATE_EDIT_VALUES
                           MAX_FEEDRATE_EDIT_VALUES
                         #else
                           default_max_feedrate * float(DEFAULT_MAX_MULTIPLIER)
                         #endif
                       ;

//
// Acceleration limits
//
#define MIN_ACCELERATION_EDIT_VALUE 1

constexpr xyze_float_t min_acceleration_edit_values = LOGICAL_AXIS_ARRAY_1(MIN_ACCELERATION_EDIT_VALUE),
                       default_max_acceleration = DEFAULT_MAX_ACCELERATION,
                       max_acceleration_edit_values =
                        #ifdef MAX_ACCEL_EDIT_VALUES
                          MAX_ACCEL_EDIT_VALUES
                        #else
                          default_max_acceleration * float(DEFAULT_MAX_MULTIPLIER)
                        #endif
                      ;

//
// Max jerk limits
//
#define MIN_JERK_EDIT_VALUE 0.1
#define DEFAULT_MAX_JERK_MULTIPLIER 2

#if ENABLED(CLASSIC_JERK)
  constexpr xyze_float_t min_jerk_edit_values = LOGICAL_AXIS_ARRAY_1(MIN_JERK_EDIT_VALUE),
                         default_jerk = LOGICAL_AXIS_ARRAY(
                           DEFAULT_EJERK,
                           DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK,
                           DEFAULT_IJERK, DEFAULT_JJERK, DEFAULT_KJERK,
                           DEFAULT_UJERK, DEFAULT_VJERK, DEFAULT_WJERK
                         ),
                         max_jerk_edit_values =
                           #ifdef MAX_JERK_EDIT_VALUES
                             MAX_JERK_EDIT_VALUES
                           #else
                             default_jerk * float(DEFAULT_MAX_JERK_MULTIPLIER)
                           #endif
                         ;
#endif

//
// Steps per mm limits
//
#define MIN_STEPS_EDIT_VALUE 1

constexpr xyze_float_t min_steps_edit_values = LOGICAL_AXIS_ARRAY_1(MIN_STEPS_EDIT_VALUE),
                       default_steps = DEFAULT_AXIS_STEPS_PER_UNIT,
                       max_steps_edit_values =
                         #ifdef MAX_STEPS_EDIT_VALUES
                           MAX_STEPS_EDIT_VALUES
                         #else
                           default_steps * float(DEFAULT_MAX_MULTIPLIER)
                         #endif
                       ;
