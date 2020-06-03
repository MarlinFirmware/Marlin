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

// Unknown thermistor for the Zonestar P802M hot bed. Adjusted By Nerseth
// These were the shipped settings from Zonestar in original firmware: P802M_8_Repetier_V1.6_Zonestar.zip
const short temptable_502[][2] PROGMEM = {
   { OV(14     ), 300 }, // 56 (the OV number is a quarter of these)
   { OV(46.75  ), 250 }, // 187
   { OV(153.75 ), 190 }, // 615
   { OV(172.5  ), 185 }, // 690
   { OV(187.5  ), 180 }, // 750
   { OV(207.5  ), 175 }, // 830
   { OV(230    ), 170 }, // 920
   { OV(252.5  ), 165 }, // 1010
   { OV(279.5  ), 160 }, // 1118
   { OV(303.75 ), 155 }, // 1215
   { OV(332.5  ), 145 }, // 1330
   { OV(365    ), 140 }, // 1460
   { OV(398.5  ), 135 }, // 1594
   { OV(438    ), 130 }, // 1752
   { OV(475    ), 125 }, // 1900
   { OV(510    ), 120 }, // 2040
   { OV(550    ), 115 }, // 2200
   { OV(587.5  ), 110 }, // 2350
   { OV(629    ), 105 }, // 2516
   { OV(667.75 ),  98 }, // 2671
   { OV(707.75 ),  92 }, // 2831
   { OV(743.75 ),  85 }, // 2975
   { OV(778.75 ),  76 }, // 3115
   { OV(812.75 ),  72 }, // 3251
   { OV(870    ),  62 }, // 3480
   { OV(895    ),  52 }, // 3580
   { OV(915    ),  46 }, // 3660
   { OV(935    ),  40 }, // 3740
   { OV(967.25 ),  30 }, // 3869
   { OV(978    ),  25 }, // 3912
   { OV(987    ),  20 }, // 3948
   { OV(1019.25), -20 }, // 4077
   { OV(1023.5 ), -55 }  // 4094
};
