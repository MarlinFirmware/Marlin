/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

 // 100k bed thermistor in JGAurora A5. Calibrated by Sam Pinches 21st Jan 2018 using cheap k-type thermocouple inserted into heater block, using TM-902C meter.
const short temptable_15[][2] PROGMEM = {
  { OV(  31), 275 },
  { OV(  33), 270 },
  { OV(  35), 260 },
  { OV(  38), 253 },
  { OV(  41), 248 },
  { OV(  48), 239 },
  { OV(  56), 232 },
  { OV(  66), 222 },
  { OV(  78), 212 },
  { OV(  93), 206 },
  { OV( 106), 199 },
  { OV( 118), 191 },
  { OV( 130), 186 },
  { OV( 158), 176 },
  { OV( 187), 167 },
  { OV( 224), 158 },
  { OV( 270), 148 },
  { OV( 321), 137 },
  { OV( 379), 127 },
  { OV( 446), 117 },
  { OV( 518), 106 },
  { OV( 593),  96 },
  { OV( 668),  86 },
  { OV( 739),  76 },
  { OV( 767),  72 },
  { OV( 830),  62 },
  { OV( 902),  48 },
  { OV( 926),  45 },
  { OV( 955),  35 },
  { OV( 966),  30 },
  { OV( 977),  25 },
  { OV( 985),  20 },
  { OV( 993),  15 },
  { OV( 999),  10 },
  { OV(1004),   5 },
  { OV(1008),   0 },
  { OV(1012),  -5 },
  { OV(1016), -10 },
  { OV(1020), -15 }
};
