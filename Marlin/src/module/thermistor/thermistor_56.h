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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

// R25 = 100 kOhm, beta25 = 4267 K, 1 kOhm pull-up,
// 100k ATC Semitec 104GT-2 (Used on ParCan) (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: https://www.mouser.com/datasheet/2/362/semitec%20usa%20corporation_gtthermistor-1202937.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const temp_entry_t temptable_56[] PROGMEM = {
  { OV(   1), 713 },
  { OV(  54), 236 },
  { OV(  107), 195 },
  { OV( 160), 172 },
  { OV( 213), 157 },
  { OV( 266), 144 },
  { OV( 319), 134 },
  { OV( 372), 125 },
  { OV( 425), 117 },
  { OV( 478), 110 },
  { OV( 531), 103 },
  { OV( 584), 96 },
  { OV( 637), 89 },
  { OV( 690), 83 },
  { OV( 743), 75 },
  { OV( 796), 68 },
  { OV( 849), 59 },
  { OV( 902), 48 },
  { OV( 955), 37 },
  { OV( 1008), 6 },
 
};
