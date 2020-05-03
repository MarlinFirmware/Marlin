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

// R25 = 100 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up,
// Formbot / Vivedino high temp 100k thermistor
// 100KR13950181203
// Generated with modified version of https://www.thingiverse.com/thing:103668
// Using table 1 with datasheet values
// Resistance         100k Ohms at 25deg. C
// Resistance Tolerance     + / -1%
// B Value             3950K at 25/50 deg. C
// B Value Tolerance         + / - 1%
const short temptable_61[][2] PROGMEM = {
  { OV(   2.00), 420 }, // Guestimate to ensure we dont lose a reading and drop temps to -50 when over
  { OV(  12.07), 350 },
  { OV(  12.79), 345 },
  { OV(  13.59), 340 },
  { OV(  14.44), 335 },
  { OV(  15.37), 330 },
  { OV(  16.38), 325 },
  { OV(  17.46), 320 },
  { OV(  18.63), 315 },
  { OV(  19.91), 310 },
  { OV(  21.29), 305 },
  { OV(  22.79), 300 },
  { OV(  24.43), 295 },
  { OV(  26.21), 290 },
  { OV(  28.15), 285 },
  { OV(  30.27), 280 },
  { OV(  32.58), 275 },
  { OV(  35.10), 270 },
  { OV(  38.44), 265 },
  { OV(  40.89), 260 },
  { OV(  44.19), 255 },
  { OV(  47.83), 250 },
  { OV(  51.80), 245 },
  { OV(  56.20), 240 },
  { OV(  61.00), 235 },
  { OV(  66.30), 230 },
  { OV(  72.11), 225 },
  { OV(  78.51), 220 },
  { OV(  85.57), 215 },
  { OV(  93.34), 210 },
  { OV( 101.91), 205 },
  { OV( 111.34), 200 },
  { OV( 121.73), 195 },
  { OV( 133.17), 190 },
  { OV( 145.74), 185 },
  { OV( 159.57), 180 },
  { OV( 174.73), 175 },
  { OV( 191.35), 170 },
  { OV( 209.53), 165 },
  { OV( 229.35), 160 },
  { OV( 250.90), 155 },
  { OV( 274.25), 150 },
  { OV( 299.46), 145 },
  { OV( 326.52), 140 },
  { OV( 355.44), 135 },
  { OV( 386.15), 130 },
  { OV( 418.53), 125 },
  { OV( 452.43), 120 },
  { OV( 487.62), 115 },
  { OV( 523.82), 110 },
  { OV( 560.70), 105 },
  { OV( 597.88), 100 },
  { OV( 634.97),  95 },
  { OV( 671.55),  90 },
  { OV( 707.21),  85 },
  { OV( 741.54),  80 },
  { OV( 779.65),  75 },
  { OV( 809.57),  70 },
  { OV( 833.40),  65 },
  { OV( 859.55),  60 },
  { OV( 883.27),  55 },
  { OV( 904.53),  50 },
  { OV( 923.38),  45 },
  { OV( 939.91),  40 },
  { OV( 954.26),  35 },
  { OV( 966.59),  30 },
  { OV( 977.08),  25 },
  { OV( 985.92),  20 },
  { OV( 993.39),  15 },
  { OV( 999.42),  10 },
  { OV(1004.43),   5 },
  { OV(1008.51),   0 },
  { OV(1011.79),  -5 },
  { OV(1014.40), -10 },
  { OV(1016.48), -15 },
  { OV(1018.10), -20 },
  { OV(1019.35), -25 },
  { OV(1020.32), -30 },
  { OV(1021.05), -35 },
  { OV(1021.60), -40 },
  { OV(1022.01), -45 },
  { OV(1022.31), -50 }
};
