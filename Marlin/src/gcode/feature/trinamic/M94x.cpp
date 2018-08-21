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

#include "../../../inc/MarlinConfig.h"

#if HAS_TRINAMIC

#include "../../gcode.h"
#include "../../../module/stepper.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper_indirection.h"

/**
 * M941: TMC set MSLUT. 
 */
#if ENABLED(TMC_MSLUT_CALIBRATION)
  #if ENABLED(TMC2130_LINEARITY_CORRECTION)
    void GcodeSuite::M941() {
      if (parser.seenval('E')) {
        #if AXIS_IS_TMC(E0)
          tmc2130_set_wave(stepperE0, TMC2130_LINEARITY_CORRECTION_AMPL, (int16_t)parser.value_int());
        #endif
      }
      if (parser.seenval('X')) {
        #if AXIS_IS_TMC(X)
          tmc2130_set_wave(stepperX, TMC2130_LINEARITY_CORRECTION_AMPL, (int16_t)parser.value_int());
        #endif
        #if AXIS_IS_TMC(X2)
          tmc2130_set_wave(stepperX2, TMC2130_LINEARITY_CORRECTION_AMPL, (int16_t)parser.value_int());
        #endif
      }
      if (parser.seenval('Y')) {
        #if AXIS_IS_TMC(Y)
          tmc2130_set_wave(stepperY, TMC2130_LINEARITY_CORRECTION_AMPL, (int16_t)parser.value_int());
        #endif
        #if AXIS_IS_TMC(Y2)
          tmc2130_set_wave(stepperY2, TMC2130_LINEARITY_CORRECTION_AMPL, (int16_t)parser.value_int());
        #endif
      }
      if (parser.seenval('Z')) {
        #if AXIS_IS_TMC(Z)
          tmc2130_set_wave(stepperZ, TMC2130_LINEARITY_CORRECTION_AMPL, (int16_t)parser.value_int());
        #endif
        #if AXIS_IS_TMC(Z2)
          tmc2130_set_wave(stepperZ2, TMC2130_LINEARITY_CORRECTION_AMPL, (int16_t)parser.value_int());
        #endif
      }
    }

/**
 * M942: TMC reset MSLUT. 
 */
    void GcodeSuite::M942() {
      if (parser.seen('E')) {
        #if AXIS_IS_TMC(E0)
          tmc2130_reset_wave(stepperE0);
        #endif
      }
      if (parser.seen('X')) {
        #if AXIS_IS_TMC(X)
          tmc2130_reset_wave(stepperX);
        #endif
        #if AXIS_IS_TMC(X2)
          tmc2130_reset_wave(stepperX2);
        #endif
      }
      if (parser.seen('Y')) {
        #if AXIS_IS_TMC(Y)
          tmc2130_reset_wave(stepperY);
        #endif
        #if AXIS_IS_TMC(Y2)
          tmc2130_reset_wave(stepperY2);
        #endif
      }
      if (parser.seen('Z')) {
        #if AXIS_IS_TMC(Z)
          tmc2130_reset_wave(stepperZ);
        #endif
        #if AXIS_IS_TMC(Z2)
          tmc2130_reset_wave(stepperZ2);
        #endif
      }
    }

/**
 * M943: TMC set preset MSLUT. 
 */
    void GcodeSuite::M943() {
      if (parser.seenval('E')) {
        #if AXIS_IS_TMC(E0)
          tmc2130_set_fixed_wave(stepperE0, (uint8_t)parser.value_int());
        #endif
      }
      if (parser.seenval('X')) {
        #if AXIS_IS_TMC(X)
          tmc2130_set_fixed_wave(stepperX, (uint8_t)parser.value_int());
        #endif
        #if AXIS_IS_TMC(X2)
          tmc2130_set_fixed_wave(stepperX2, (uint8_t)parser.value_int());
        #endif
      }
      if (parser.seenval('Y')) {
        #if AXIS_IS_TMC(Y)
          tmc2130_set_fixed_wave(stepperY, (uint8_t)parser.value_int());
        #endif
        #if AXIS_IS_TMC(Y2)
          tmc2130_set_fixed_wave(stepperY2, (uint8_t)parser.value_int());
        #endif
      }
      if (parser.seenval('Z')) {
        #if AXIS_IS_TMC(Z)
          tmc2130_set_fixed_wave(stepperZ, (uint8_t)parser.value_int());
        #endif
        #if AXIS_IS_TMC(Z2)
          tmc2130_set_fixed_wave(stepperZ2, (uint8_t)parser.value_int());
        #endif
      }
    }
  #endif // TMC2130_LINEARITY_CORRECTION
#endif // TMC_MSLUT_CALIBRATION

/**
 * M945: TMC switch StealthChop.
 */
#if ENABLED(TMC_CHOP_MODE)
void GcodeSuite::M945() {
 if (parser.seenval('E')) {
        #if AXIS_IS_TMC(E0)
          stepperE0.stealthChop(parser.value_bool());
        #endif
      }
      if (parser.seenval('X')) {
        #if AXIS_IS_TMC(X)
          stepperX.stealthChop(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(X2)
          stepperX2.stealthChop(parser.value_bool());
        #endif
      }
      if (parser.seenval('Y')) {
        #if AXIS_IS_TMC(Y)
          stepperY.stealthChop(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(Y2)
          stepperY2.stealthChop(parser.value_bool());
        #endif
      }
      if (parser.seenval('Z')) {
        #if AXIS_IS_TMC(Z)
          stepperZ.stealthChop(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(Z2)
          stepperZ2.stealthChop(parser.value_bool());
        #endif
      }
}
#endif // TMC_CHOP_MODE

/**
 * M946: TMC switch ChopperMode.
 */
#if ENABLED(TMC_CHOP_MODE)
void GcodeSuite::M946() {
 if (parser.seenval('E')) {
        #if AXIS_IS_TMC(E0)
          stepperE0.chopper_mode(parser.value_bool());
        #endif
      }
      if (parser.seenval('X')) {
        #if AXIS_IS_TMC(X)
          stepperX.chopper_mode(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(X2)
          stepperX2.chopper_mode(parser.value_bool());
        #endif
      }
      if (parser.seenval('Y')) {
        #if AXIS_IS_TMC(Y)
          stepperY.chopper_mode(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(Y2)
          stepperY2.chopper_mode(parser.value_bool());
        #endif
      }
      if (parser.seenval('Z')) {
        #if AXIS_IS_TMC(Z)
          stepperZ.chopper_mode(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(Z2)
          stepperZ2.chopper_mode(parser.value_bool());
        #endif
      }
}
#endif // TMC_CHOP_MODE

/**
 * M947: TMC switch interpolation.
 */
#if ENABLED(TMC_CHOP_MODE)
void GcodeSuite::M947() {
 if (parser.seenval('E')) {
        #if AXIS_IS_TMC(E0)
          stepperE0.interpolate(parser.value_bool());
        #endif
      }
      if (parser.seenval('X')) {
        #if AXIS_IS_TMC(X)
          stepperX.interpolate(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(X2)
          stepperX2.interpolate(parser.value_bool());
        #endif
      }
      if (parser.seenval('Y')) {
        #if AXIS_IS_TMC(Y)
          stepperY.interpolate(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(Y2)
          stepperY2.interpolate(parser.value_bool());
        #endif
      }
      if (parser.seenval('Z')) {
        #if AXIS_IS_TMC(Z)
          stepperZ.interpolate(parser.value_bool());
        #endif
        #if AXIS_IS_TMC(Z2)
          stepperZ2.interpolate(parser.value_bool());
        #endif
      }
}
#endif // TMC_CHOP_MODE

#endif // HAS_TRINAMIC
