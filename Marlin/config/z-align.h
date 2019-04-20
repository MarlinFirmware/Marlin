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
 * Z Steppers Auto-Alignment
 * Add the G34 command to align multiple Z steppers using a bed probe.
 */
//#define Z_STEPPER_AUTO_ALIGN
#if ENABLED(Z_STEPPER_AUTO_ALIGN)
  // Define probe X and Y positions for Z1, Z2 [, Z3]
  #define Z_STEPPER_ALIGN_X { 10, 150, 290 }
  #define Z_STEPPER_ALIGN_Y { 290, 10, 290 }
  // Set number of iterations to align
  #define Z_STEPPER_ALIGN_ITERATIONS 3
  // Enable to restore leveling setup after operation
  #define RESTORE_LEVELING_AFTER_G34
  // Use the amplification factor to de-/increase correction step.
  // In case the stepper (spindle) position is further out than the test point
  // Use a value > 1. NOTE: This may cause instability
  #define Z_STEPPER_ALIGN_AMP 1.0
  // Stop criterion. If the accuracy is better than this stop iterating early
  #define Z_STEPPER_ALIGN_ACC 0.02
#endif
