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

static void tmc2130_get_pwmthrs(TMC2130Stepper &st, const char name, const uint16_t spmm) {
  SERIAL_CHAR(name);
  SERIAL_ECHOPGM(" stealthChop max speed set to ");
  SERIAL_ECHOLN(12650000UL * st.microsteps() / (256 * st.stealth_max_speed() * spmm));
}
static void tmc2130_set_pwmthrs(TMC2130Stepper &st, const char name, const int32_t thrs, const uint32_t spmm) {
  st.stealth_max_speed(12650000UL * st.microsteps() / (256 * thrs * spmm));
  tmc2130_get_pwmthrs(st, name, spmm);
}

/**
 * M913: Set HYBRID_THRESHOLD speed.
 */
void gcode_M913() {
  uint16_t values[XYZE];
  LOOP_XYZE(i)
    values[i] = parser.intval(axis_codes[i]);

  #if ENABLED(X_IS_TMC2130)
    if (values[X_AXIS]) tmc2130_set_pwmthrs(stepperX, 'X', values[X_AXIS], planner.axis_steps_per_mm[X_AXIS]);
    else tmc2130_get_pwmthrs(stepperX, 'X', planner.axis_steps_per_mm[X_AXIS]);
  #endif
  #if ENABLED(Y_IS_TMC2130)
    if (values[Y_AXIS]) tmc2130_set_pwmthrs(stepperY, 'Y', values[Y_AXIS], planner.axis_steps_per_mm[Y_AXIS]);
    else tmc2130_get_pwmthrs(stepperY, 'Y', planner.axis_steps_per_mm[Y_AXIS]);
  #endif
  #if ENABLED(Z_IS_TMC2130)
    if (values[Z_AXIS]) tmc2130_set_pwmthrs(stepperZ, 'Z', values[Z_AXIS], planner.axis_steps_per_mm[Z_AXIS]);
    else tmc2130_get_pwmthrs(stepperZ, 'Z', planner.axis_steps_per_mm[Z_AXIS]);
  #endif
  #if ENABLED(E0_IS_TMC2130)
    if (values[E_AXIS]) tmc2130_set_pwmthrs(stepperE0, 'E', values[E_AXIS], planner.axis_steps_per_mm[E_AXIS]);
    else tmc2130_get_pwmthrs(stepperE0, 'E', planner.axis_steps_per_mm[E_AXIS]);
  #endif
}
