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

static void tmc2130_clear_otpw(TMC2130Stepper &st, const char name) {
  st.clear_otpw();
  SERIAL_CHAR(name);
  SERIAL_ECHOLNPGM(" prewarn flag cleared");
}

/**
 * M912: Clear TMC2130 stepper driver overtemperature pre-warn flag held by the library
 */
void gcode_M912() {
  const bool clearX = parser.seen('X'), clearY = parser.seen('Y'), clearZ = parser.seen('Z'), clearE = parser.seen('E'),
           clearAll = (!clearX && !clearY && !clearZ && !clearE) || (clearX && clearY && clearZ && clearE);
  #if ENABLED(X_IS_TMC2130)
    if (clearX || clearAll) tmc2130_clear_otpw(stepperX, 'X');
  #endif
  #if ENABLED(Y_IS_TMC2130)
    if (clearY || clearAll) tmc2130_clear_otpw(stepperY, 'Y');
  #endif
  #if ENABLED(Z_IS_TMC2130)
    if (clearZ || clearAll) tmc2130_clear_otpw(stepperZ, 'Z');
  #endif
  #if ENABLED(E0_IS_TMC2130)
    if (clearE || clearAll) tmc2130_clear_otpw(stepperE0, 'E');
  #endif
}
