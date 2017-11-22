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

// bqh2 stock thermistor
const short temptable_70[][2] PROGMEM = {
  { OV(  22), 300 },
  { OV(  24), 295 },
  { OV(  25), 290 },
  { OV(  27), 285 },
  { OV(  29), 280 },
  { OV(  32), 275 },
  { OV(  34), 270 },
  { OV(  37), 265 },
  { OV(  40), 260 },
  { OV(  43), 255 },
  { OV(  46), 250 },
  { OV(  50), 245 },
  { OV(  54), 240 },
  { OV(  59), 235 },
  { OV(  64), 230 },
  { OV(  70), 225 },
  { OV(  76), 220 },
  { OV(  83), 215 },
  { OV(  90), 210 },
  { OV(  99), 205 },
  { OV( 108), 200 },
  { OV( 118), 195 },
  { OV( 129), 190 },
  { OV( 141), 185 },
  { OV( 154), 180 },
  { OV( 169), 175 },
  { OV( 185), 170 },
  { OV( 203), 165 },
  { OV( 222), 160 },
  { OV( 243), 155 },
  { OV( 266), 150 },
  { OV( 290), 145 },
  { OV( 317), 140 },
  { OV( 346), 135 },
  { OV( 376), 130 },
  { OV( 408), 125 },
  { OV( 442), 120 },
  { OV( 477), 115 },
  { OV( 513), 110 },
  { OV( 551), 105 },
  { OV( 588), 100 },
  { OV( 626),  95 },
  { OV( 663),  90 },
  { OV( 699),  85 },
  { OV( 735),  80 },
  { OV( 768),  75 },
  { OV( 800),  70 },
  { OV( 829),  65 },
  { OV( 856),  60 },
  { OV( 881),  55 },
  { OV( 903),  50 },
  { OV( 922),  45 },
  { OV( 939),  40 },
  { OV( 954),  35 },
  { OV( 966),  30 },
  { OV( 977),  25 },
  { OV( 986),  20 },
  { OV( 994),  15 },
  { OV(1000),  10 },
  { OV(1005),   5 },
  { OV(1009),   0 } // safety
};
