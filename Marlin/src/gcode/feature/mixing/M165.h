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
 * M165: Set multiple mix factors for a mixing extruder.
 *       Factors that are left out will be set to 0.
 *       All factors together must add up to 1.0.
 *
 *   A[factor] Mix factor for extruder stepper 1
 *   B[factor] Mix factor for extruder stepper 2
 *   C[factor] Mix factor for extruder stepper 3
 *   D[factor] Mix factor for extruder stepper 4
 *   H[factor] Mix factor for extruder stepper 5
 *   I[factor] Mix factor for extruder stepper 6
 *
 */
void gcode_M165() { gcode_get_mix(); }
