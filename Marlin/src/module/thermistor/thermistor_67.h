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

// R25 = 500 KOhm, beta25 = 3800 K, 4.7 kOhm pull-up, SliceEngineering 450 °C Thermistor
const short temptable_67[][2] PROGMEM = {
  { OV(  22 ),  500 },
  { OV(  23 ),  490 },
  { OV(  25 ),  480 },
  { OV(  27 ),  470 },
  { OV(  29 ),  460 },
  { OV(  32 ),  450 },
  { OV(  35 ),  440 },
  { OV(  38 ),  430 },
  { OV(  41 ),  420 },
  { OV(  45 ),  410 },
  { OV(  50 ),  400 },
  { OV(  55 ),  390 },
  { OV(  60 ),  380 },
  { OV(  67 ),  370 },
  { OV(  74 ),  360 },
  { OV(  82 ),  350 },
  { OV(  91 ),  340 },
  { OV( 102 ),  330 },
  { OV( 114 ),  320 },
  { OV( 127 ),  310 },
  { OV( 143 ),  300 },
  { OV( 161 ),  290 },
  { OV( 181 ),  280 },
  { OV( 204 ),  270 },
  { OV( 229 ),  260 },
  { OV( 259 ),  250 },
  { OV( 290 ),  240 },
  { OV( 325 ),  230 },
  { OV( 364 ),  220 },
  { OV( 407 ),  210 },
  { OV( 453 ),  200 },
  { OV( 501 ),  190 },
  { OV( 551 ),  180 },
  { OV( 603 ),  170 },
  { OV( 655 ),  160 },
  { OV( 706 ),  150 },
  { OV( 755 ),  140 },
  { OV( 801 ),  130 },
  { OV( 842 ),  120 },
  { OV( 879 ),  110 },
  { OV( 910 ),  100 },
  { OV( 936 ),   90 },
  { OV( 948 ),   85 },
  { OV( 958 ),   80 },
  { OV( 975 ),   70 },
  { OV( 988 ),   60 },
  { OV( 998 ),   50 },
  { OV(1006 ),   40 },
  { OV(1011 ),   30 },
  { OV(1013 ),   25 },
  { OV(1015 ),   20 },
  { OV(1018 ),   10 },
  { OV(1020 ),    0 },
  { OV(1021 ),  -10 },
  { OV(1022 ),  -20 }
};
