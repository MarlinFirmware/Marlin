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

// 100k 0603 SMD Vishay NTCS0603E3104FXT (4.7k pullup)
const short temptable_8[][2] PROGMEM = {
  { OV(   1), 704 },
  { OV(  54), 216 },
  { OV( 107), 175 },
  { OV( 160), 152 },
  { OV( 213), 137 },
  { OV( 266), 125 },
  { OV( 319), 115 },
  { OV( 372), 106 },
  { OV( 425),  99 },
  { OV( 478),  91 },
  { OV( 531),  85 },
  { OV( 584),  78 },
  { OV( 637),  71 },
  { OV( 690),  65 },
  { OV( 743),  58 },
  { OV( 796),  50 },
  { OV( 849),  42 },
  { OV( 902),  31 },
  { OV( 955),  17 },
  { OV(1008),   0 }
};
