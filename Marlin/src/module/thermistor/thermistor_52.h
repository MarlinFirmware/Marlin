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

// R25 = 200 kOhm, beta25 = 4338 K, 1 kOhm pull-up,
// 200k ATC Semitec 204GT-2 (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: https://www.mouser.com/datasheet/2/362/semitec%20usa%20corporation_gtthermistor-1202937.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const temp_entry_t temptable_52[] PROGMEM = {
  { OV(   1), 500 },
  { OV( 125), 300 }, // top rating 300C
  { OV( 142), 290 },
  { OV( 162), 280 },
  { OV( 185), 270 },
  { OV( 211), 260 },
  { OV( 240), 250 },
  { OV( 274), 240 },
  { OV( 312), 230 },
  { OV( 355), 220 },
  { OV( 401), 210 },
  { OV( 452), 200 },
  { OV( 506), 190 },
  { OV( 563), 180 },
  { OV( 620), 170 },
  { OV( 677), 160 },
  { OV( 732), 150 },
  { OV( 783), 140 },
  { OV( 830), 130 },
  { OV( 871), 120 },
  { OV( 906), 110 },
  { OV( 935), 100 },
  { OV( 958),  90 },
  { OV( 976),  80 },
  { OV( 990),  70 },
  { OV(1000),  60 },
  { OV(1008),  50 },
  { OV(1013),  40 },
  { OV(1017),  30 },
  { OV(1019),  20 },
  { OV(1021),  10 },
  { OV(1022),   0 }
};
