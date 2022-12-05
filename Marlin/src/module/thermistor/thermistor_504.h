/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

// QWG 104F B3950 thermistor
constexpr temp_entry_t temptable_504[] PROGMEM = {
  { OV(  15), 330 },
  { OV(  17), 315 },
  { OV(  19), 300 },
  { OV(  20), 295 },
  { OV(  21), 290 },
  { OV(  23), 285 },
  { OV(  25), 280 },
  { OV(  27), 275 },
  { OV(  28), 270 },
  { OV(  31), 265 },
  { OV(  33), 260 },
  { OV(  35), 255 },
  { OV(  38), 250 },
  { OV(  41), 245 },
  { OV(  44), 240 },
  { OV(  48), 235 },
  { OV(  52), 230 },
  { OV(  56), 225 },
  { OV(  61), 220 },
  { OV(  66), 215 },
  { OV(  78), 210 },
  { OV(  92), 205 },
  { OV( 100), 200 },
  { OV( 109), 195 },
  { OV( 120), 190 },
  { OV( 143), 185 },
  { OV( 148), 180 },
  { OV( 156), 175 },
  { OV( 171), 170 },
  { OV( 187), 165 },
  { OV( 205), 160 },
  { OV( 224), 155 },
  { OV( 268), 150 },
  { OV( 293), 145 },
  { OV( 320), 140 },
  { OV( 348), 135 },
  { OV( 379), 130 },
  { OV( 411), 125 },
  { OV( 445), 120 },
  { OV( 480), 115 },
  { OV( 516), 110 },
  { OV( 553), 105 },
  { OV( 591), 100 },
  { OV( 628),  95 },
  { OV( 665),  90 },
  { OV( 702),  85 },
  { OV( 737),  80 },
  { OV( 770),  75 },
  { OV( 801),  70 },
  { OV( 830),  65 },
  { OV( 857),  60 },
  { OV( 881),  55 },
  { OV( 903),  50 },
  { OV( 922),  45 },
  { OV( 939),  40 },
  { OV( 954),  35 },
  { OV( 966),  30 },
  { OV( 977),  25 },
  { OV( 985),  23 },
  { OV( 993),  20 },
  { OV( 999),  18 },
  { OV(1004),  15 },
  { OV(1008),  12 },
  { OV(1012),   8 },
  { OV(1016),   5 },
  { OV(1020),   0 },
  { OV(1023),  -5 }
};
