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

// R25 = 100 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up
// Resistance         100k Ohms at 25deg. C
// Resistance Tolerance     + / -1%
// B Value             3950K at 25/50 deg. C
// B Value Tolerance         + / - 1%
// Kis3d Silicone Heater 24V 200W/300W with 6mm Precision cast plate (EN AW 5083)
// Temperature setting time 10 min to determine the 12Bit ADC value on the surface. (le3tspeak)
const temp_entry_t temptable_30[] PROGMEM = {
  { OV(   1), 938 },
  { OV( 298), 125 }, // 1193 - 125°
  { OV( 321), 121 }, // 1285 - 121°
  { OV( 348), 117 }, // 1392 - 117°
  { OV( 387), 113 }, // 1550 - 113°
  { OV( 411), 110 }, // 1644 - 110°
  { OV( 445), 106 }, // 1780 - 106°
  { OV( 480), 101 }, // 1920 - 101°
  { OV( 516),  97 }, // 2064 - 97°
  { OV( 553),  92 }, // 2212 - 92°
  { OV( 591),  88 }, // 2364 - 88°
  { OV( 628),  84 }, // 2512 - 84°
  { OV( 665),  79 }, // 2660 - 79°
  { OV( 702),  75 }, // 2808 - 75°
  { OV( 736),  71 }, // 2945 - 71°
  { OV( 770),  67 }, // 3080 - 67°
  { OV( 801),  63 }, // 3204 - 63°
  { OV( 830),  59 }, // 3320 - 59°
  { OV( 857),  55 }, // 3428 - 55°
  { OV( 881),  51 }, // 3524 - 51°
  { OV( 902),  47 }, // 3611 - 47°
  { OV( 922),  42 }, // 3688 - 42°
  { OV( 938),  38 }, // 3754 - 38°
  { OV( 952),  34 }, // 3811 - 34°
  { OV( 964),  29 }, // 3857 - 29°
  { OV( 975),  25 }, // 3900 - 25°
  { OV( 980),  23 }, // 3920 - 23°
  { OV( 991),  17 }, // 3964 - 17°
  { OV(1001),   9 }, // Calculated
  { OV(1004),   5 }, // Calculated
  { OV(1008),   0 }, // Calculated
  { OV(1012),  -5 }, // Calculated
  { OV(1016), -10 }, // Calculated
  { OV(1020), -15 }  // Calculated
};
