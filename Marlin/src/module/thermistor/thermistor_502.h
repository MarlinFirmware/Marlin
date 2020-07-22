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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

// Unknown thermistor for the Zonestar P802M hot bed. Adjusted By Nerseth
// These were the shipped settings from Zonestar in original firmware: P802M_8_Repetier_V1.6_Zonestar.zip
const temp_entry_t temptable_502[] PROGMEM = {
   { OV(  56.0 / 4), 300 },
   { OV( 187.0 / 4), 250 },
   { OV( 615.0 / 4), 190 },
   { OV( 690.0 / 4), 185 },
   { OV( 750.0 / 4), 180 },
   { OV( 830.0 / 4), 175 },
   { OV( 920.0 / 4), 170 },
   { OV(1010.0 / 4), 165 },
   { OV(1118.0 / 4), 160 },
   { OV(1215.0 / 4), 155 },
   { OV(1330.0 / 4), 145 },
   { OV(1460.0 / 4), 140 },
   { OV(1594.0 / 4), 135 },
   { OV(1752.0 / 4), 130 },
   { OV(1900.0 / 4), 125 },
   { OV(2040.0 / 4), 120 },
   { OV(2200.0 / 4), 115 },
   { OV(2350.0 / 4), 110 },
   { OV(2516.0 / 4), 105 },
   { OV(2671.0 / 4),  98 },
   { OV(2831.0 / 4),  92 },
   { OV(2975.0 / 4),  85 },
   { OV(3115.0 / 4),  76 },
   { OV(3251.0 / 4),  72 },
   { OV(3480.0 / 4),  62 },
   { OV(3580.0 / 4),  52 },
   { OV(3660.0 / 4),  46 },
   { OV(3740.0 / 4),  40 },
   { OV(3869.0 / 4),  30 },
   { OV(3912.0 / 4),  25 },
   { OV(3948.0 / 4),  20 },
   { OV(4077.0 / 4), -20 },
   { OV(4094.0 / 4), -55 }
};
