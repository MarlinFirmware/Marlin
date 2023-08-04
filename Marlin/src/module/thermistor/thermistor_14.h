/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// R25 = 100 kOhm, beta25 = 4092 K, 4.7 kOhm pull-up, bed thermistor
const temp_entry_t temptable_14[] PROGMEM = {
  { OV(  23), 275 },
  { OV(  25), 270 },
  { OV(  27), 265 },
  { OV(  28), 260 },
  { OV(  31), 255 },
  { OV(  33), 250 },
  { OV(  35), 245 },
  { OV(  38), 240 },
  { OV(  41), 235 },
  { OV(  44), 230 },
  { OV(  47), 225 },
  { OV(  52), 220 },
  { OV(  56), 215 },
  { OV(  62), 210 },
  { OV(  68), 205 },
  { OV(  74), 200 },
  { OV(  81), 195 },
  { OV(  90), 190 },
  { OV(  99), 185 },
  { OV( 108), 180 },
  { OV( 121), 175 },
  { OV( 133), 170 },
  { OV( 147), 165 },
  { OV( 162), 160 },
  { OV( 180), 155 },
  { OV( 199), 150 },
  { OV( 219), 145 },
  { OV( 243), 140 },
  { OV( 268), 135 },
  { OV( 296), 130 },
  { OV( 326), 125 },
  { OV( 358), 120 },
  { OV( 398), 115 },
  { OV( 435), 110 },
  { OV( 476), 105 },
  { OV( 519), 100 },
  { OV( 566),  95 },
  { OV( 610),  90 },
  { OV( 658),  85 },
  { OV( 703),  80 },
  { OV( 742),  75 },
  { OV( 773),  70 },
  { OV( 807),  65 },
  { OV( 841),  60 },
  { OV( 871),  55 },
  { OV( 895),  50 },
  { OV( 918),  45 },
  { OV( 937),  40 },
  { OV( 954),  35 },
  { OV( 968),  30 },
  { OV( 978),  25 },
  { OV( 985),  20 },
  { OV( 993),  15 },
  { OV( 999),  10 },
  { OV(1004),   5 },
  { OV(1008),   0 },
  { OV(1012),  -5 },
  { OV(1016), -10 },
  { OV(1020), -15 }
};
