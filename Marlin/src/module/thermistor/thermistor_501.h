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

// 100k Zonestar thermistor. Adjusted By Hally
const short temptable_501[][2] PROGMEM = {
   { OV(   1), 713 },
   { OV(  14), 300 }, // Top rating 300C
   { OV(  16), 290 },
   { OV(  19), 280 },
   { OV(  23), 270 },
   { OV(  27), 260 },
   { OV(  31), 250 },
   { OV(  37), 240 },
   { OV(  47), 230 },
   { OV(  57), 220 },
   { OV(  68), 210 },
   { OV(  84), 200 },
   { OV( 100), 190 },
   { OV( 128), 180 },
   { OV( 155), 170 },
   { OV( 189), 160 },
   { OV( 230), 150 },
   { OV( 278), 140 },
   { OV( 336), 130 },
   { OV( 402), 120 },
   { OV( 476), 110 },
   { OV( 554), 100 },
   { OV( 635),  90 },
   { OV( 713),  80 },
   { OV( 784),  70 },
   { OV( 846),  60 },
   { OV( 897),  50 },
   { OV( 937),  40 },
   { OV( 966),  30 },
   { OV( 986),  20 },
   { OV(1000),  10 },
   { OV(1010),   0 }
};
