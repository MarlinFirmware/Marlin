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

static void tmc2130_report_otpw(TMC2130Stepper &st, const char name) {
  SERIAL_CHAR(name);
  SERIAL_ECHOPGM(" axis temperature prewarn triggered: ");
  serialprintPGM(st.getOTPW() ? PSTR("true") : PSTR("false"));
  SERIAL_EOL();
}

/**
 * M911: Report TMC2130 stepper driver overtemperature pre-warn flag
 * The flag is held by the library and persist until manually cleared by M912
 */
void gcode_M911() {
  const bool reportX = parser.seen('X'), reportY = parser.seen('Y'), reportZ = parser.seen('Z'), reportE = parser.seen('E'),
           reportAll = (!reportX && !reportY && !reportZ && !reportE) || (reportX && reportY && reportZ && reportE);
  #if ENABLED(X_IS_TMC2130)
    if (reportX || reportAll) tmc2130_report_otpw(stepperX, 'X');
  #endif
  #if ENABLED(Y_IS_TMC2130)
    if (reportY || reportAll) tmc2130_report_otpw(stepperY, 'Y');
  #endif
  #if ENABLED(Z_IS_TMC2130)
    if (reportZ || reportAll) tmc2130_report_otpw(stepperZ, 'Z');
  #endif
  #if ENABLED(E0_IS_TMC2130)
    if (reportE || reportAll) tmc2130_report_otpw(stepperE0, 'E');
  #endif
}
